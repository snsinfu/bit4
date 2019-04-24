import keras
import keras.layers as L
import numpy as np

from trainers import Trainer


def main():
    input_x1 = L.Input((2,))
    input_x2 = L.Input((2,))

    net = L.concatenate([input_x1, input_x2])
    net = L.Dense(20, activation="elu")(net)
    output_y1 = L.Dense(1, activation="sigmoid", name="y1")(net)
    output_y2 = L.Dense(1, activation="sigmoid", name="y2")(net)

    model = keras.Model([input_x1, input_x2], [output_y1, output_y2])
    model.compile(loss="binary_crossentropy", optimizer=keras.optimizers.Adam(0.01))

    valid_X, valid_y = generate_dataset(1000, a=-2, b=0)

    trainer = Trainer(
        model,
        steps_per_epoch=10,
        validation_data=(valid_X, valid_y),
        callbacks=[keras.callbacks.ProgbarLogger("steps")],
        verbose=1,
    )
    for _ in trainer.epochs(20):
        X, y = generate_dataset(10000, a=-1, b=1)
        trainer.train(X, y)


def generate_dataset(n, a=-2, b=2):
    x1 = np.random.uniform(a, b, size=(n, 2))
    x2 = np.random.uniform(a, b, size=(n, 2))
    y1 = x1[:, 0] > x2[:, 1]
    y2 = x1[:, 1] > x2[:, 0]
    X = [x1, x2]
    y = [y1, y2]
    return X, y


if __name__ == "__main__":
    main()
