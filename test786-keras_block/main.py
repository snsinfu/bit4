import keras
import keras.backend as K
import keras.layers as L
import numpy as np


def main():
    """
    model = keras.Sequential(
        [
            L.InputLayer((100, 2)),
            TCN(10, kernel_size=5, dilation_rates=[1, 2, 4, 8, 16], activation="linear"),
            L.GlobalAveragePooling1D(),
            L.Dense(1, activation="sigmoid"),
        ]
    )
    """

    """
    model = Classifier()
    """

    tcn = LayerBlock()
    tcn.add(L.Dense(10, kernel_regularizer=keras.regularizers.l2(0.1)))
    conv_options = {"kernel_size": 5, "padding": "same", "activation": "linear"}
    tcn.add(L.Conv1D(10, dilation_rate=1, **conv_options))
    tcn.add(L.Conv1D(10, dilation_rate=2, **conv_options))
    tcn.add(L.Conv1D(10, dilation_rate=4, **conv_options))
    tcn.add(L.Conv1D(10, dilation_rate=8, **conv_options))
    tcn.add(L.Conv1D(10, dilation_rate=16, **conv_options))

    model = keras.Sequential(
        [
            L.InputLayer((100, 2)),
            tcn,
            L.GlobalAveragePooling1D(),
            L.Dense(1, activation="sigmoid"),
        ]
    )

    """
    shaper = L.Dense(10, kernel_regularizer=keras.regularizers.l2(0.1))
    conv_options = {"kernel_size": 5, "padding": "same", "activation": "linear"}
    conv1 = L.Conv1D(10, dilation_rate=1, **conv_options)
    conv2 = L.Conv1D(10, dilation_rate=2, **conv_options)
    conv3 = L.Conv1D(10, dilation_rate=4, **conv_options)
    conv4 = L.Conv1D(10, dilation_rate=8, **conv_options)
    conv5 = L.Conv1D(10, dilation_rate=16, **conv_options)
    norm = L.BatchNormalization()
    model = keras.Sequential([
        L.InputLayer((100, 2)),
        shaper, conv1, conv2, conv3, conv4, conv5, norm,
        L.GlobalAveragePooling1D(),
        L.Dense(1, activation="sigmoid"),
    ])
    """

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


class MyTCN(keras.Model):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.shaper = L.Dense(10, kernel_regularizer=keras.regularizers.l2(0.1))
        conv_options = {"kernel_size": 5, "padding": "same", "activation": "linear"}
        self.conv1 = L.Conv1D(10, dilation_rate=1, **conv_options)
        self.conv2 = L.Conv1D(10, dilation_rate=2, **conv_options)
        self.conv3 = L.Conv1D(10, dilation_rate=4, **conv_options)
        self.conv4 = L.Conv1D(10, dilation_rate=8, **conv_options)
        self.conv5 = L.Conv1D(10, dilation_rate=16, **conv_options)
        self.norm = L.BatchNormalization()

    def compute_output_shape(self, input_shape):
        return input_shape[:-1] + (self.shaper.units,)

    def call(self, x):
        net = self.shaper(x)
        net = self.conv1(net)
        net = self.conv2(net)
        net = self.conv3(net)
        net = self.conv4(net)
        net = self.conv5(net)
        net = self.norm(net)
        return net


class DenseL2(keras.Model):
    def __init__(self, units, **kwargs):
        super().__init__(**kwargs)
        self.dense = L.Dense(units, kernel_regularizer=keras.regularizers.l2(0.1))

    def compute_output_shape(self, input_shape):
        return input_shape[:-1] + (self.dense.units,)

    def call(self, x):
        return self.dense(x)


class Classifier(keras.Model):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        # self.shaper = L.Dense(10, kernel_regularizer=keras.regularizers.l2(0.1))
        self.shaper = DenseL2(10)
        conv_options = {"kernel_size": 5, "padding": "same", "activation": "linear"}
        self.conv1 = L.Conv1D(10, dilation_rate=1, **conv_options)
        self.conv2 = L.Conv1D(10, dilation_rate=2, **conv_options)
        self.conv3 = L.Conv1D(10, dilation_rate=4, **conv_options)
        self.conv4 = L.Conv1D(10, dilation_rate=8, **conv_options)
        self.conv5 = L.Conv1D(10, dilation_rate=16, **conv_options)
        self.pool = L.GlobalAveragePooling1D()
        self.output_dense = L.Dense(1, activation="sigmoid")

    def compute_output_shape(self, input_shape):
        return input_shape[0], self.output_dense.units

    def call(self, x):
        net = self.shaper(x)
        net += self.conv1(net)
        net += self.conv2(net)
        net += self.conv3(net)
        net += self.conv4(net)
        net += self.conv5(net)
        net = self.pool(net)
        return self.output_dense(net)


class LayerBlock(keras.Model):
    def __init__(self, residual=False, **kwargs):
        super().__init__(**kwargs)
        self._residual = residual
        self._chain = []

    def add(self, layer):
        self._chain.append(layer)
        setattr(self, layer.name, layer)

    def build(self, input_shape):
        shape = input_shape
        for layer in self._chain:
            layer.build(shape)
            shape = layer.compute_output_shape(shape)
        super().build(input_shape)

    def compute_output_shape(self, input_shape):
        shape = input_shape
        for layer in self._chain:
            shape = layer.compute_output_shape(shape)
        return shape

    def call(self, x):
        y = x
        for layer in self._chain:
            y = layer(y)
        if self._residual:
            y = x + y
        return y

    @property
    def losses(self):
        l = super().losses
        for layer in self._chain:
            l.extend(layer.losses)
        return l

    # !!! THIS
    # This enables regularization on nested layers. I'm still unsure of the API
    # though. What is inputs?
    def get_losses_for(self, inputs):
        l = super().get_losses_for(inputs)
        for layer in self._chain:
            l.extend(layer.get_losses_for(inputs))
        return l


class TCN(keras.Model):
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
