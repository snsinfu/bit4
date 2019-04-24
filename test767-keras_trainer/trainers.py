import keras
import keras.layers as L
import numpy as np


def sizeof_dataset(data):
    if isinstance(data, (list, tuple)):
        return len(data[0])
    return len(data)


def slice_dataset(data, beg, end):
    if isinstance(data, (list, tuple)):
        return [subdata[beg:end] for subdata in data]
    return data[beg:end]


def generate_batches(X, y, batch_size):
    """
    Generate batch samples from given dataset. If y is None the batch samples for y
    become a sequence of Nones.
    """
    sample_size = sizeof_dataset(X)
    batch_count = (sample_size + batch_size - 1) // batch_size
    for i in range(batch_count):
        beg = i * batch_size
        end = min(beg + batch_size, sample_size)
        batch_X = slice_dataset(X, beg, end)
        if y is None:
            batch_y = None
        else:
            batch_y = slice_dataset(y, beg, end)
        yield batch_X, batch_y


def make_metrics_dict(model, scalar_outputs):
    """
    Translates scalar outputs from Keras' Model methods to a dict of metrics.
    """
    if isinstance(scalar_outputs, list):
        return dict(zip(model.metrics_names, scalar_outputs))
    else:
        name = model.metrics_names[0]
        return {name: scalar_outputs}


class Trainer:
    def __init__(
        self, model, steps_per_epoch, validation_data=None, callbacks=None, **kwargs
    ):
        self._model = model
        self._steps_per_epoch = steps_per_epoch
        self._validation_data = validation_data
        self._callbacks = callbacks or []
        self._extra_params = kwargs

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
        metrics_names = self._model.metrics_names
        if self._validation_data is not None:
            metrics_names.extend([f"val_{name}" for name in metrics_names])

        train_params = {
            "epochs": epochs,
            "steps": self._steps_per_epoch,
            "metrics": metrics_names,
        }
        train_params.update(self._extra_params)

        # BaseLogger is required to record metrics that are used by other callbacks.
        # History is for convenience.
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

        sample_size = sizeof_dataset(X)
        batch_size = (sample_size + self._steps_per_epoch - 1) // self._steps_per_epoch
        train_batches = generate_batches(X, y, batch_size)

        for step, (batch_X, batch_y) in enumerate(train_batches):
            batch_logs = {"batch": step, "size": 1}
            self._callback_list.on_batch_begin(step, batch_logs)

            scalar_outputs = self._model.train_on_batch(batch_X, batch_y)
            metrics = make_metrics_dict(self._model, scalar_outputs)
            batch_logs.update(metrics)

            self._callback_list.on_batch_end(step, batch_logs)

        if self._validation_data:
            if y is None:
                valid_X = self._validation_data
                valid_y = None
            else:
                valid_X, valid_y = self._validation_data
            valid_metrics = self.test(valid_X, valid_y, batch_size)
            valid_metrics = {f"val_{name}": val for name, val in valid_metrics.items()}
            epoch_logs.update(valid_metrics)

        # CallbackList replaces empty logs argument by a new local dict, rendering
        # logs argument useless. So fill in something here. The "loss" item is filled
        # by the BaseLogger callback so let us use "loss" key. We cannot use arbitrary
        # dummy key because the History callback records everything in the dict.
        epoch_logs["loss"] = None

        self._callback_list.on_epoch_end(self._epoch, epoch_logs)
        self._epoch += 1

        return epoch_logs

    def test(self, X, y=None, batch_size=32):
        steps = 0
        test_metrics = {}
        test_batches = generate_batches(X, y, batch_size)

        for batch_X, batch_y in test_batches:
            scalar_outputs = self._model.test_on_batch(batch_X, batch_y)
            metrics = make_metrics_dict(self._model, scalar_outputs)

            if test_metrics:
                test_metrics = {
                    name: metrics[name] + test_metrics[name] for name in metrics
                }
            else:
                test_metrics = metrics
            steps += 1

        for name in test_metrics:
            test_metrics[name] = test_metrics[name] / steps

        return test_metrics

    @property
    def history(self):
        return self._history.history
