import keras
import keras.backend as K
import keras.layers as L
import numpy as np


def batch_reshape(x, shape):
    batch = K.shape(x)[0]
    shape_tensor = K.stack([batch] + list(shape))
    return K.reshape(x, shape_tensor)


inputs = L.Input((90,))
shape = 9, 10
outputs = L.Lambda(lambda x: batch_reshape(x, shape), output_shape=shape)(inputs)
model = keras.Model(inputs, outputs)

X = np.arange(90).reshape(1, 90)
y = model.predict(X)
print(y[0])
