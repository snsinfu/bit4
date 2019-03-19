import argparse
import os
import signal

import keras
import keras.layers as L
import numpy as np

from models import IntClassifier


integer_bits = 16


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    run(**parse_args())


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("weights", type=str, default="weights.h5")
    return vars(parser.parse_args())


def run(weights):
    model = IntClassifier(classes=4, hidden_dim=30, hidden_stacks=3, dropout=0.5)

    if os.access(weights, mode=os.R_OK):
        # call() is required to build layers before loading weights.
        model.call(L.Input((integer_bits,)))
        model.load_weights(weights)
    else:
        train_leap_year(model)
        model.save_weights(weights)

    test_model(model)


def test_model(model):
    numbers = [1000, 1200, 2000, 2004, 2006, 2019, 2020, 2100, 2400]
    X_test = np.array([encode_binary(n, bits=integer_bits) for n in numbers])
    y_pred = model.predict(X_test)

    print("n\tpred\ttrue")
    for n, p in zip(numbers, y_pred):
        print(f"{n}\t{np.argmax(p)}\t{compute_year_label(n)}")


def train_leap_year(model):
    numbers = list(range(1, 2001)) + list(range(100, 2001, 100)) * 100
    numbers = np.array(numbers)
    np.random.shuffle(numbers)
    labels = np.array([compute_year_label(n) for n in numbers])
    X_train = np.array([encode_binary(n, bits=integer_bits) for n in numbers])
    y_train = keras.utils.to_categorical(labels)

    optimizer = keras.optimizers.Adam(lr=0.01)
    model.compile(
        loss="binary_crossentropy", optimizer=optimizer, metrics=["accuracy"]
    )

    def lr_schedule(epoch, lr):
        if epoch % 9 == 0:
            lr *= 0.5
        return lr

    train_options = {
        "batch_size": 32,
        "epochs": 30,
        "validation_split": 0.1,
        "verbose": 2,
        "callbacks": [keras.callbacks.LearningRateScheduler(lr_schedule)],
    }
    model.fit(X_train, y_train, **train_options)


def encode_binary(n, bits):
    return [(n >> i) & 1 for i in range(bits)]


def compute_year_label(n):
    if n % 400 == 0:
        return 3
    if n % 100 == 0:
        return 2
    if n % 4 == 0:
        return 1
    return 0


if __name__ == "__main__":
    main()
