import keras
import keras.layers as L

from modules import ResidualFC


class IntClassifier(keras.Model):
    """
    MLP classifier for binary-encoded integers.
    """
    def __init__(self, classes, hidden_dim, hidden_stacks=1, dropout=0, **kwargs):
        super().__init__(**kwargs)

        inner_dim = hidden_dim * hidden_dim // 2

        self.frontend_inner_dense = L.Dense(inner_dim)
        self.frontend_activation = L.ReLU()
        self.frontend_outer_dense = L.Dense(hidden_dim)
        self.res_modules = [
            ResidualFC(hidden_dim, inner_dim, dropout=dropout)
            for _ in range(hidden_stacks)
        ]
        self.output_layer = L.Dense(classes, activation="sigmoid")

    def call(self, inputs):
        net = inputs

        net = self.frontend_inner_dense(net)
        net = self.frontend_activation(net)
        net = self.frontend_outer_dense(net)

        for res_module in self.res_modules:
            net = res_module(net)

        output = self.output_layer(net)
        return output
