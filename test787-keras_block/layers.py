import keras
import keras.layers as L


class Module(L.Layer):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._tracked_layers = []

    def __setattr__(self, name, value):
        if isinstance(value, L.Layer):
            self._tracked_layers.append(value)
        return super().__setattr__(name, value)

    @property
    def trainable_weights(self):
        w = super().trainable_weights
        w = w + self._gather_child_attribute("trainable_weights")
        return w

    @property
    def non_trainable_weights(self):
        w = super().non_trainable_weights
        w = w + self._gather_child_attribute("non_trainable_weights")
        return w

    @property
    def losses(self):
        l = list(super().losses)
        l += self._gather_child_attribute("losses")
        return l

    @property
    def updates(self):
        u = list(super().updates)
        u += self._gather_child_attribute("updates")
        return u

    def get_losses_for(self, inputs=None):
        # BUG: Incorrect when losses are conditional on their inputs. Layer graph
        # needs to be known. Still works for unconditional losses like weight
        # regularizers.
        l = super().get_losses_for(inputs)
        for layer in self._tracked_layers:
            l = l + layer.get_losses_for(inputs)
        return l

    def get_updates_for(self, inputs=None):
        # BUG: Same as above. Conditional updates like batch normalization moving
        # statistics are not updated.
        u = super().get_updates_for(inputs)
        for layer in self._tracked_layers:
            u = u + layer.get_updates_for(inputs)
        return u

    def _gather_child_attribute(self, name):
        result = []
        for layer in self._tracked_layers:
            result += getattr(layer, name)
        return result


class LayerBlock(Module):
    def __init__(self, residual=False, **kwargs):
        super().__init__(**kwargs)
        self._residual = residual
        self._chain = []

    def add(self, layer):
        self._chain.append(layer)
        setattr(self, layer.name, layer)

    def build(self, input_shape):
        shape = input_shape
        for layer in self._chain:
            layer.build(shape)
            shape = layer.compute_output_shape(shape)
        super().build(input_shape)

    def compute_output_shape(self, input_shape):
        shape = input_shape
        for layer in self._chain:
            shape = layer.compute_output_shape(shape)
        return shape

    def call(self, x):
        y = x
        for layer in self._chain:
            y = layer(y)
        if self._residual:
            y += x
        return y
