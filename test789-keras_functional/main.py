import keras
import keras.backend as K
import keras.layers as L
import numpy as np

from modules import TCN


# Both regularization and batch normalization works with the functional API.
# Maybe I should stick with it.


def main():
    model = keras.Sequential(
        [
            L.InputLayer((100, 2)),
            TCN(
                (100, 2),
                10,
                kernel_size=5,
                dilation_rates=[1, 2, 4, 8, 16],
                norm="batchnorm",
                front_l2=0.01,  # l2 seems harmful for this model though
                name="tcn"
            ),
            L.GlobalAveragePooling1D(name="pool"),
            L.Dense(1, activation="sigmoid", name="output"),
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


if __name__ == "__main__":
    main()
