import keras.backend as K
import keras.layers as L


class MyTimeDistributed(L.Wrapper):
    def __init__(self, layer, **kwargs):
        super().__init__(layer, **kwargs)

    def build(self, input_shape):
        step_shape = (input_shape[0],) + input_shape[2:]
        self.timesteps = input_shape[1]
        self.layer.build(step_shape)
        super().build()

    def compute_output_shape(self, input_shape):
        timesteps = input_shape[1]
        inner_input_shape = input_shape[:1] + input_shape[2:]
        inner_output_shape = self.layer.compute_output_shape(inner_input_shape)
        return inner_output_shape[:1] + (timesteps,) + inner_output_shape[1:]

    def call(self, inputs):
        #
        # (batch, timesteps, input_chan)
        #  ↓ Reshape
        # (batch * timesteps, input_chan)
        #  ↓ Layer
        # (batch * timesteps, output_chan)
        #  ↓ Reshape
        # (batch, timesteps, output_chan)
        #
        input_shape = K.int_shape(inputs)
        output_shape = self.compute_output_shape(input_shape)
        inner_inputs = K.reshape(inputs, (-1,) + input_shape[2:])
        inner_outputs = self.layer.call(inner_inputs)
        return K.reshape(inner_outputs, (-1,) + output_shape[1:])
