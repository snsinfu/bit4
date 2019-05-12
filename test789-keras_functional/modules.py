import keras
import keras.layers as L
import keras.backend as K

from layers import LayerNormalization


def TCN(
    input_shape, filters, kernel_size, dilation_rates, front_l2=0, norm=None, dropout=0, **kwargs
):
    x = L.Input(input_shape)

    net = x
    net = L.Dense(filters, kernel_regularizer=keras.regularizers.l2(front_l2))(net)

    for d in dilation_rates:
        res = net

        net = L.Conv1D(
            filters, kernel_size, dilation_rate=d, padding="same", activation="elu"
        )(net)

        if norm == "batchnorm":
            net = L.BatchNormalization()(net)
        if norm == "layernorm":
            net = LayerNormalization()(net)

        if dropout > 0:
            net = L.SpatialDropout1D(dropout)(net)

        net = L.add([res, net])

    out = net

    return keras.Model(x, net, **kwargs)
