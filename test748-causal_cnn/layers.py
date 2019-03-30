import keras
import keras.backend as K
import keras.layers as L

import tensorflow as tf


class CausalDepthwiseConv1D(L.Layer):
    def __init__(
        self,
        kernel_size,
        depth_multiplier=1,
        dilation_rate=1,
        kernel_initializer="glorot_uniform",
        **kwargs
    ):
        super().__init__(**kwargs)

        self.kernel_size = kernel_size
        self.depth_multiplier = depth_multiplier
        self.dilation_rate = dilation_rate
        self.kernel_initializer = kernel_initializer

    def build(self, input_shape):
        _, _, input_dim = input_shape
        self.kernel = self.add_weight(
            shape=(self.kernel_size, input_dim, self.depth_multiplier),
            initializer=self.kernel_initializer,
            name="kernel",
        )
        super().build(input_shape)

    def compute_output_shape(self, input_shape):
        batch, timesteps, input_dim = input_shape
        output_dim = input_dim * self.depth_multiplier
        return batch, timesteps, output_dim

    def call(self, x):
        # Causal pad
        left_pad = self.dilation_rate * (self.kernel_size - 1)
        x = K.temporal_padding(x, (left_pad, 0))

        # As 1xk convolution on 2D data
        x = K.expand_dims(x, axis=1)
        kernel = K.expand_dims(self.kernel, axis=0)
        dilation_rate = (1, self.dilation_rate)
        x = tf.nn.depthwise_conv2d(
            x,
            kernel,
            strides=(1, 1, 1, 1),
            rate=dilation_rate,
            padding="VALID",
            data_format="NHWC",
        )
        x = K.squeeze(x, axis=1)

        return x
