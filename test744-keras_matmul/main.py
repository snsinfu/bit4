import keras
import keras.backend as K
import keras.layers as L
import numpy as np


a_len = 100
b_len = 200
a_dim = 7
b_dim = 9
key_dim = 11
value_dim = 13
heads = 19

input_a = L.Input((a_len, a_dim))
input_b = L.Input((b_len, b_dim))

q = L.Dense(heads * key_dim)(input_a)
k = L.Dense(heads * key_dim)(input_b)
v = L.Dense(heads * value_dim)(input_b)

q = L.Reshape((a_len, heads, key_dim))(q)
k = L.Reshape((b_len, heads, key_dim))(k)
v = L.Reshape((b_len, heads, value_dim))(v)

q = L.Permute((2, 1, 3))(q) # (*, heads, a_len, key_dim)
k = L.Permute((2, 1, 3))(k) # (*, heads, b_len, key_dim)
v = L.Permute((2, 3, 1))(v) # (*, heads, value_dim, b_len)

def matmul(q, k):
    # q (batch, heads, Lq, d)
    # k (batch, heads, Lk, d)
    # --> (batch, heads, Lq, Lk)
    return K.batch_dot(q, k, axes=[3, 3])

qk = L.Lambda(lambda args: matmul(*args), output_shape=(heads, a_len, b_len))([q, k])
o = L.Lambda(lambda args: matmul(*args), output_shape=(heads, a_len, value_dim))([qk, v])
o = L.Permute((2, 1, 3))(o) # (*, a_len, heads, value_dim)

model = keras.Model([input_a, input_b], o)

a = np.random.uniform(size=(3, a_len, a_dim))
b = np.random.uniform(size=(3, b_len, b_dim))
y = model.predict([a, b])

print(y.shape)
