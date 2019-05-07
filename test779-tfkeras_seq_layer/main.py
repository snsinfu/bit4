import tensorflow.keras as keras
import tensorflow.keras.layers as L


class LayerBlock(L.Layer):
    def __init__(self, residual=False, **kwargs):
        super().__init__(**kwargs)
        self._residual = False
        self._chain = []

    def add(self, layer):
        setattr(self, layer.name, layer)
        self._chain.append(layer)

    def call(self, x):
        y = x
        for layer in self._chain:
            y = layer(y)
        if self._residual:
            y = x + y
        return y


model = keras.Sequential()
model.add(L.InputLayer((20, 3)))

for i in range(4):
    step = LayerBlock(residual=True, name=f"step{i + 1}")
    step.add(L.Conv1D(10, kernel_size=5, padding="same", activation="elu"))
    step.add(L.BatchNormalization())
    step.add(L.SpatialDropout1D(0.1))
    model.add(step)

model.add(L.GlobalAveragePooling1D())
model.add(L.Dense(1, activation="sigmoid"))
model.summary()

model.save_weights("weights.h5")
model.load_weights("weights.h5")
