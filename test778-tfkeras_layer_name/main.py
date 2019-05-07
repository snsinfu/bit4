import tensorflow.keras as keras
import tensorflow.keras.layers as L


input_layer = L.InputLayer((100, 3))
conv1 = L.Conv1D(5, kernel_size=3, activation="elu")
norm1 = L.BatchNormalization()

# Name layers after construction. Does this work? => No. There is no setter.
input_layer.name = "input"
conv1.name = "conv1"
norm1.name = "norm1"

model = keras.Sequential([input_layer, conv1, norm1])
model.summary()
print(model.to_yaml())
