import logging
import os

logging.getLogger("tensorflow").setLevel(logging.ERROR)
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"  # Suppress INFO and WARNING
os.environ["KMP_AFFINITY"] = "scatter,none"  # Suppress verbose log

import keras
import keras.layers as L
import numpy as np


def main():
    model = keras.Sequential()
    model.add(L.Dense(10, activation="elu", input_dim=2))
    model.add(L.Dense(1, activation="sigmoid"))
    model.compile(loss="binary_crossentropy", optimizer=keras.optimizers.Adam(lr=0.01))
    train(model, epochs=10, batch_size=200, batch_count=500)


def train(model, epochs, batch_size, batch_count):
    history_callback = keras.callbacks.History()
    callbacks = [
        keras.callbacks.BaseLogger(),
        keras.callbacks.ProgbarLogger("steps"),
        history_callback,
    ]

    callback_list = keras.callbacks.CallbackList(callbacks)
    callback_list.set_model(model)
    callback_list.set_params(
        {
            "epochs": epochs,
            "verbose": 1,
            "steps": batch_count,
            "metrics": model.metrics_names,
        }
    )

    callback_list.on_train_begin()
    for epoch_index in range(epochs):
        callback_list.on_epoch_begin(epoch_index)
        epoch_logs = {"loss": None}

        for batch_index in range(batch_count):
            batch_logs = {"batch": batch_index, "size": 1}
            callback_list.on_batch_begin(batch_index, batch_logs)

            # Generate training batch
            x = np.random.uniform(-3, 3, size=(batch_size, 2))
            y = x[:, 1] > x[:, 0]
            loss = model.train_on_batch(x, y)

            batch_logs["loss"] = loss
            callback_list.on_batch_end(batch_index, batch_logs)

        callback_list.on_epoch_end(epoch_index, epoch_logs)
    callback_list.on_train_end()

    return history_callback.history


if __name__ == "__main__":
    main()
