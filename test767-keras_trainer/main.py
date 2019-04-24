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

    trainer = Trainer(
        model,
        steps_per_epoch=10,
        callbacks=[keras.callbacks.ProgbarLogger("steps")],
        verbose=1,
    )
    for _ in trainer.epochs(20):
        x1 = np.random.uniform(-2, 2, size=(100, 2))
        x2 = np.random.uniform(-2, 2, size=(100, 2))
        y1 = x1[:, 0] > x2[:, 1]
        y2 = x1[:, 1] > x2[:, 0]
        X = [x1, x2]
        y = [y1, y2]
        trainer.train(X, y)


if __name__ == "__main__":
    main()
