import keras
import keras.backend as K
import keras.layers as L
import numpy as np

from layers import LayerBlock, Module


# Weight regularization works but batch normalization does not work!


def main():
    tcn = TCN(10, kernel_size=5, dilation_rates=[1, 2, 4, 8, 16])
    model = keras.Sequential(
        [
            L.InputLayer((100, 2)),
            tcn,
            L.Dense(1, activation="sigmoid"),
        ]
    )
    model.summary()

    model.compile(
        loss="binary_crossentropy",
        optimizer=keras.optimizers.Adam(0.01),
        metrics=["accuracy", "binary_crossentropy"],
    )

    X = np.random.uniform(-1, 1, size=(5000, 100, 2))
    a = np.mean(X[:, :50, 0] + X[:, 50:, 1], axis=1)
    b = np.mean(X[:, :50, 1] + X[:, 50:, 0], axis=1)
    y = a < b
    model.fit(X, y, batch_size=100, epochs=10, validation_split=0.1)


class TCN(Module):
    def __init__(self, encoding_dim, kernel_size, dilation_rates, **kwargs):
        super().__init__(**kwargs)
        self.frontend = L.Dense(encoding_dim, kernel_regularizer="l2", name="frontend")
        self.tcn_steps = LayerBlock(name="steps")
        for i, d in enumerate(dilation_rates):
            step = LayerBlock(residual=True, name=f"step{i + 1}")
            step.add(
                L.Conv1D(
                    encoding_dim,
                    kernel_size,
                    dilation_rate=d,
                    padding="same",
                    activation="elu",
                    kernel_constraint=keras.constraints.MaxNorm(1),
                )
            )
            self.tcn_steps.add(step)
        self.pool = L.GlobalAveragePooling1D(name="pool")

    def build(self, input_shape):
        self.frontend.build(input_shape)
        encoding_shape = self.frontend.compute_output_shape(input_shape)
        self.tcn_steps.build(encoding_shape)
        self.pool.build(encoding_shape)
        super().build(input_shape)

    def compute_output_shape(self, input_shape):
        return input_shape[0], self.frontend.units

    def call(self, x):
        return self.pool(self.tcn_steps(self.frontend(x)))


if __name__ == "__main__":
    main()
