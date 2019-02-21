import keras.backend as K
import keras.layers as L


class FreeParameters(L.Layer):
    def __init__(
        self,
        shape,
        initializer="glorot_uniform",
        regularizer=None,
        constraint=None,
        **kwargs
    ):
        self._shape = shape
        self._initializer = initializer
        self._regularizer = regularizer
        self._constraint = constraint
        super(FreeParameters, self).__init__(**kwargs)

    def build(self, input_shape):
        self._params = self.add_weight(
            name="params",
            shape=self._shape,
            initializer=self._initializer,
            regularizer=self._regularizer,
            constraint=self._constraint,
        )
        super(FreeParameters, self).build(input_shape)

    def call(self, x):
        return K.expand_dims(self._params, axis=0)

    def compute_output_shape(self, input_shape):
        return self._shape
