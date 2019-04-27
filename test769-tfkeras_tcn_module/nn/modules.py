import types

import tensorflow.keras.layers as L

from .layers import LayerNormalization


class SymmetricTCN(L.Layer):
    def __init__(
        self,
        filters,
        kernel_size,
        dilation_rates,
        depth_multiplier=1,
        activation="elu",
        dropout=0,
        depthwise_initializer="glorot_uniform",
        pointwise_initializer="glorot_uniform",
        depthwise_regularizer=None,
        pointwise_regularizer=None,
        **kwargs,
    ):
        super().__init__(**kwargs)

        self.steps = []
        for i, dilation_rate in enumerate(dilation_rates):
            step = types.SimpleNamespace()
            step_name = f"step{i + 1}"

            step.conv1 = L.SeparableConv1D(
                filters,
                kernel_size=kernel_size,
                dilation_rate=dilation_rate,
                depth_multiplier=depth_multiplier,
                depthwise_initializer=depthwise_initializer,
                depthwise_regularizer=depthwise_regularizer,
                pointwise_initializer=pointwise_initializer,
                pointwise_regularizer=pointwise_regularizer,
                activation=activation,
                padding="same",
                name=f"{step_name}_dilated_conv",
            )
            step.norm1 = LayerNormalization(name=f"{step_name}_dilated_conv_norm")

            step.conv2 = L.SeparableConv1D(
                filters,
                kernel_size=kernel_size,
                depth_multiplier=depth_multiplier,
                depthwise_initializer=depthwise_initializer,
                depthwise_regularizer=depthwise_regularizer,
                pointwise_initializer=pointwise_initializer,
                pointwise_regularizer=pointwise_regularizer,
                activation=activation,
                padding="same",
                name=f"{step_name}_local_conv",
            )
            step.norm2 = LayerNormalization(name=f"{step_name}_local_conv_norm")
            step.dropout = L.SpatialDropout1D(dropout, name=f"{step_name}_dropout")

            self.steps.append(step)

            # HACK-ish: Make child layers tracked. Keras tracks child layers only when
            # they are set as attributes. And no other API is provided for making child
            # layers tracked. See:
            # https://github.com/tensorflow/tensorflow/commit/f0c4d5814ad6ecbba06bd813d94a2fefb2caba3d
            setattr(self, f"{step_name}_dilated_conv", step.conv1)
            setattr(self, f"{step_name}_dilated_conv_norm", step.norm1)
            setattr(self, f"{step_name}_local_conv", step.conv2)
            setattr(self, f"{step_name}_local_conv_norm", step.norm2)
            setattr(self, f"{step_name}_dropout", step.dropout)

    def call(self, x):
        net = x
        for step in self.steps:
            res = net
            net = step.conv1(net)
            net = step.norm1(net)
            net = step.conv2(net)
            net = step.norm2(net)
            net = step.dropout(net)
            net = res + net
        return net
