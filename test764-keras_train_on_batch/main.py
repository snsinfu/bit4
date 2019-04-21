import logging
import os

# Suppress INFO and WARNING from Tensorflow
logging.getLogger("tensorflow").setLevel(logging.ERROR)
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"

# Suppress verbose log from Intel OMP
os.environ["KMP_AFFINITY"] = "scatter,none"


import keras
import keras.layers as L
import numpy as np


def main():
    model = keras.Sequential()
    model.add(L.Dense(10, activation="elu", input_dim=2))
    model.add(L.Dense(1, activation="sigmoid"))
    model.compile(loss="binary_crossentropy", optimizer=keras.optimizers.Adam(lr=0.01))

    trainer = Trainer(
        model,
        steps_per_epoch=500,
        verbose=1,
        callbacks=[keras.callbacks.ProgbarLogger("steps")],
    )

    for epoch in trainer.epochs(10):
        X = np.random.uniform(-3, 3, size=(10000, 2))
        y = X[:, 0] < X[:, 1]
        trainer.train(X, y)

    print(trainer.history)


class Trainer:
    def __init__(self, model, steps_per_epoch, callbacks=None, **kwargs):
        self._model = model
        self._steps_per_epoch = steps_per_epoch
        self._callbacks = callbacks or []
        self._extra_params = kwargs

        # TODO: Take validation data as argument.

    def epochs(self, epochs):
        def do_training():
            self.start(epochs)
            for epoch in range(epochs):
                try:
                    yield epoch
                except GeneratorExit:
                    break
            self.finish()

        return do_training()

    def start(self, epochs):
        train_params = {
            "epochs": epochs,
            "steps": self._steps_per_epoch,
            "metrics": self._model.metrics_names,
        }
        train_params.update(self._extra_params)

        # BaseLogger is required by many callbacks. History is for convenience.
        base_logger = keras.callbacks.BaseLogger()
        history = keras.callbacks.History()
        callbacks = [base_logger] + self._callbacks + [history]
        callback_list = keras.callbacks.CallbackList(callbacks)
        callback_list.set_model(self._model)
        callback_list.set_params(train_params)

        self._epochs = epochs
        self._epoch = 0
        self._history = history
        self._callback_list = callback_list
        self._callback_list.on_train_begin()

    def finish(self):
        self._callback_list.on_train_end()

    def train(self, X, y=None):
        self._callback_list.on_epoch_begin(self._epoch)
        epoch_logs = {}

        batch_size = (len(X) + self._steps_per_epoch - 1) // self._steps_per_epoch

        for step in range(self._steps_per_epoch):
            batch_logs = {"batch": step, "size": 1}
            self._callback_list.on_batch_begin(step, batch_logs)

            beg = step * batch_size
            end = min(beg + batch_size, len(X))
            batch_X = X[beg:end]
            if y is None:
                batch_y = None
            else:
                batch_y = y[beg:end]

            # TODO: Handle multiple outputs and metrics.
            loss = self._model.train_on_batch(batch_X, batch_y)
            batch_logs["loss"] = loss

            self._callback_list.on_batch_end(step, batch_logs)

        # CallbackList replaces empty logs argument by a new local dict, rendering
        # logs argument useless. So fill in something here. The "loss" item is filled
        # by the BaseLogger callback so let us use "loss" key. We cannot use a dummy
        # key because the History callback records everything in the dict.
        epoch_logs["loss"] = None

        # TODO: Compure validation loss and metrics here.

        self._callback_list.on_epoch_end(self._epoch, epoch_logs)
        self._epoch += 1

        return epoch_logs

    @property
    def history(self):
        return self._history.history


if __name__ == "__main__":
    main()
