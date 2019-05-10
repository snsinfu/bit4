import keras
import keras.backend as K
import keras.layers as L
import numpy as np


def main():
    model = keras.Sequential(
        [
            L.InputLayer((100, 2)),
            L.SpatialDropout1D(0.3),
            TCN(10, kernel_size=5, dilation_rates=[1, 2, 4, 8, 16], activation="linear"),
            L.GlobalAveragePooling1D(),
            L.Dense(1, activation="sigmoid"),
        ]
    )
    model.summary()
    model.compile(
        loss="binary_crossentropy",
        optimizer=keras.optimizers.Adam(0.01),
        metrics=["accuracy", "binary_crossentropy"],
    )

    X = np.random.uniform(-1, 1, size=(5000, 100, 2))
    a = np.mean(X[:, :50, 0] + X[:, 50:, 1], axis=1)
    b = np.mean(X[:, :50, 1] + X[:, 50:, 0], axis=1)
    y = a < b

    model.fit(X, y, batch_size=100, epochs=10, validation_split=0.1)


# XXX: Is it really sufficient to override just [non_]trainable_weights?
#
# -> NO! Weight regulariaztion is not applied when nested. Overriding losses
# property does not help. Why?

class Module(L.Layer):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._tracked_layers = {}

    def __setattr__(self, name, value):
        if isinstance(value, L.Layer):
            if name not in self._tracked_layers:
                self._tracked_layers[name] = value
        super().__setattr__(name, value)

    @property
    def losses(self):
        l = super().losses
        l = l + self._gather_child_attribute("losses")
        return l

    @property
    def updates(self):
        u = super().updates
        u = u + self._gather_child_attribute("updates")
        return u

    @property
    def trainable_weights(self):
        w = super().trainable_weights
        w = w + self._gather_child_attribute("trainable_weights")
        return w

    @property
    def non_trainable_weights(self):
        w = super().non_trainable_weights
        w = w + self._gather_child_attribute("non_trainable_weights")
        return w

    def _gather_child_attribute(self, name):
        result = []
        for layer in self._tracked_layers.values():
            result.extend(getattr(layer, name))
        return result


class LayerBlock(Module):
    def __init__(self, residual=False, **kwargs):
        super().__init__(**kwargs)
        self._residual = residual
        self._layers = []

    def add(self, layer):
        self._layers.append(layer)
        setattr(self, layer.name, layer)

    def build(self, input_shape):
        shape = input_shape
        for layer in self._layers:
            layer.build(shape)
            shape = layer.compute_output_shape(shape)
        super().build(input_shape)

    def compute_output_shape(self, input_shape):
        shape = input_shape
        for layer in self._layers:
            shape = layer.compute_output_shape(shape)
        return shape

    def call(self, x):
        y = x
        for layer in self._layers:
            y = layer(y)
        if self._residual:
            y = x + y
        return y


class TCN(Module):
    def __init__(
        self,
        filters,
        kernel_size,
        dilation_rates,
        activation="elu",
        normalization=None,
        dropout=0,
        **kwargs
    ):
        super().__init__(**kwargs)

        self.filters = filters
        self.steps = LayerBlock()
        self.steps.add(L.Dense(filters, kernel_regularizer=keras.regularizers.l2(0.1)))

        for d in dilation_rates:
            step = LayerBlock(residual=True)
            step.add(
                L.Conv1D(
                    filters,
                    kernel_size,
                    padding="same",
                    dilation_rate=d,
                    activation=activation,
                )
            )
            if normalization == "layer":
                step.add(LayerNormalization())
            if normalization == "batch":
                step.add(L.BatchNormalization())
            step.add(L.SpatialDropout1D(dropout))
            self.steps.add(step)

    def compute_output_shape(self, input_shape):
        return input_shape[:-1] + (self.filters,)

    def call(self, x):
        return self.steps(x)


class LayerNormalization(L.Layer):
    def __init__(self, epsilon=None, **kwargs):
        super().__init__(**kwargs)
        self.epsilon = epsilon or K.epsilon()

    def build(self, input_shape):
        shape = (input_shape[-1],)
        self.gamma = self.add_weight(shape=shape, initializer="ones", name="gamma")
        self.beta = self.add_weight(shape=shape, initializer="zeros", name="beta")
        super().build(input_shape)

    def compute_output_shape(self, input_shape):
        return input_shape

    def call(self, x):
        mean = K.mean(x, axis=-1, keepdims=True)
        var = K.var(x, axis=-1, keepdims=True)
        output = (x - mean) / K.sqrt(var + self.epsilon)
        output = self.gamma * output + self.beta
        return output


if __name__ == "__main__":
    main()
