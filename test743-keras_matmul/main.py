import keras
import keras.backend as K
import keras.layers as L
import numpy as np


a_len = 100
b_len = 200
a_dim = 7
b_dim = 9
key_dim = 11

input_a = L.Input((a_len, a_dim))
input_b = L.Input((b_len, b_dim))

q = L.Dense(key_dim)(input_a)
k = L.Dense(key_dim)(input_b)

# q (*, a_len, key_dim)
# k (*, b_len, key_dim)
# --> (*, a_len, b_len)

def matmul(q, k):
    return K.batch_dot(q, k, axes=[2, 2])

output = L.Lambda(lambda args: matmul(*args), output_shape=(a_len, b_len))([q, k])
model = keras.Model([input_a, input_b], output)

a = np.random.uniform(size=(3, a_len, a_dim))
b = np.random.uniform(size=(3, b_len, b_dim))
y = model.predict([a, b])
print(y.shape)
