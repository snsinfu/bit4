import numpy as np
import tensorflow.keras as keras
import tensorflow.keras.backend as K
import tensorflow.keras.layers as L


class LayerNormalization(L.Layer):
    def build(self, input_shape):
        shape = (input_shape[-1],)
        self.gamma = self.add_weight(shape=shape, initializer="ones", name="gamma")
        self.beta = self.add_weight(shape=shape, initializer="zeros", name="beta")
        self.var_epsilon = K.epsilon() ** 2
        super().build(input_shape)

    def call(self, x):
        mean = K.mean(x, axis=-1, keepdims=True)
        var = K.var(x, axis=-1, keepdims=True)
        output = (x - mean) / K.sqrt(var + self.var_epsilon)
        output = self.gamma * output + self.beta
        return output


class ResDense(L.Layer):
    def __init__(self, units, activation=None, dropout=0, **kwargs):
        super().__init__(**kwargs)

        self.dense = L.Dense(units, activation=activation)
        self.drop = L.Dropout(dropout)
        self.norm = LayerNormalization()

        self.config = {"units": units, "activation": activation, "dropout": dropout}

    def call(self, x):
        y = x
        y = self.dense(y)
        y = self.drop(y)
        y = self.norm(y)
        return x + y

    def get_config(self):
        config = super().get_config()
        config.update(self.config)
        return config


model = keras.Sequential(
    [
        L.InputLayer((5,)),
        L.Dense(20),
        ResDense(20, activation="elu", dropout=0.2),
        ResDense(20, activation="elu"),
        ResDense(20, activation="elu"),
        L.Dense(1, activation="sigmoid"),
    ]
)
model.compile(
    loss="binary_crossentropy",
    optimizer=keras.optimizers.Adam(0.01),
    metrics=["accuracy"],
)

X = np.random.normal(0, 1, size=(100000, 5))
y = (X[:, :] > 0).mean(axis=1) > 0.5
model.fit(X, y, batch_size=100, epochs=10, validation_split=0.1)

# OK
model.save_weights("weights.h5")
model.load_weights("weights.h5")

# OK
print(model.to_yaml())

# ValueError: Unknown layer: ResModule
keras.models.save_model(model, "model.h5")
model = keras.models.load_model("model.h5")
