import numpy as np
import tensorflow as tf
import tensorflow.keras as keras
import tensorflow.keras.backend as K
import tensorflow.keras.layers as L


class GumbelSoftmax(L.Layer):
    def __init__(self, temperature=1, **kwargs):
        super().__init__(**kwargs)
        self.initial_temperature = temperature

    def build(self, input_shape):
        self.temperature = K.variable(self.initial_temperature)
        super().build(input_shape)

    def call(self, x, training=None):
        shape = K.shape(x)
        u = K.random_uniform(shape)
        u = K.clip(u, K.epsilon(), 1 - K.epsilon())
        logit = x - K.log(-K.log(u))
        softmax = K.softmax(logit / self.temperature)
        hardmax = K.one_hot(K.argmax(x), shape[-1])
        hardmax = K.reshape(K.cast(hardmax, K.dtype(x)), shape)
        return K.in_train_phase(softmax, hardmax, training=training)


class DataGenerator(keras.utils.Sequence):
    def __init__(self, batch_size, batch_count, random_state=None):
        super().__init__()

        if random_state is None:
            random_state = np.random.rand.__self__

        self._batch_size = batch_size
        self._batch_count = batch_count
        self._random_state = random_state
        self._dataset = self.generate()

    @property
    def batch_size(self):
        return self._batch_size

    def __len__(self):
        return self._batch_count

    def __getitem__(self, idx):
        beg = idx * self._batch_size
        end = beg + self._batch_size
        X = self._dataset[beg:end]
        y = None
        return X, y

    def on_epoch_end(self):
        self._dataset = self.generate()

    def generate(self):
        n = (self._batch_count * self._batch_size + 1) // 2
        dataset = np.concatenate(
            [
                self._random_state.poisson(2, size=(n, 10)),
                self._random_state.poisson(9, size=(n, 10)),
            ]
        )
        self._random_state.shuffle(dataset)
        return dataset


class DummySequence(keras.utils.Sequence):
    def __init__(self, dataset, batch_size):
        super().__init__()
        self._dataset = dataset
        self._batch_size = batch_size

    def __len__(self):
        X, y = self._dataset
        return (len(X) + self._batch_size - 1) // self._batch_size

    def __getitem__(self, idx):
        X, y = self._dataset
        beg = idx * self._batch_size
        end = beg + self._batch_size
        end = min(end, len(X))
        X = X[beg:end]
        if y is not None:
            y = y[beg:end]
        return X, y


model = keras.Sequential(
    [
        L.InputLayer((10,)),
        L.Dense(20, activation="elu", kernel_regularizer=keras.regularizers.l2(0.01)),
        L.Dense(2),
        GumbelSoftmax(),
        L.Dense(1),
    ]
)

# Poisson log-likelihood reconstruction loss.
x = model.inputs[0]
loglam = model.outputs[0]

loglik = x * loglam - tf.math.exp(loglam) - tf.math.lgamma(x + 1)
loglik = K.sum(loglik, axis=1)
loglik = K.mean(loglik, axis=0)

model.add_loss(-loglik)
model.add_metric(loglik, name="loglik", aggregation="mean")
model.compile(optimizer="adam")

# Validation data needs to be a Sequence because it is unconditionally passed to
# generator-based model_iteration function [1]. Also, as of v1.13.1 progbar errorneously
# appears during validation (fixed in the next release [2]).
# [1]: https://github.com/tensorflow/tensorflow/blob/v1.13.1/tensorflow/python/keras/engine/training_generator.py#L216
# [2]: https://github.com/tensorflow/tensorflow/commit/abe1c5f6c00f7aa1ac1820ad4566a75a466663c5
data = DataGenerator(batch_size=100, batch_count=500)
val = DummySequence((data.generate(), None), batch_size=data.batch_size)
model.fit(data, epochs=20, verbose=2, validation_data=val)

# Test
X = np.concatenate(
    [
        np.random.poisson(2, size=(3, 10)),
        np.random.poisson(9, size=(3, 10)),
        np.random.poisson(4, size=(3, 10)),  # non-classifiable
    ]
)
lam = np.exp(model.predict(X))[:, 0]
print(lam)
