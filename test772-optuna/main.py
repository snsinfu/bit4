import numpy as np
import optuna
import tensorflow.keras as keras
import tensorflow.keras.backend as K
import tensorflow.keras.layers as L


def main():
    study = optuna.create_study(
        direction="maximize",
        pruner=optuna.pruners.MedianPruner(n_startup_trials=10, n_warmup_steps=5),
    )
    study.optimize(objective, n_jobs=2, n_trials=200)

    for trial in study.trials:
        print(f"{trial.number}\t{trial.user_attrs.get('test_acc')}\t{trial.params}")


def objective(trial):
    l2_weight = trial.suggest_loguniform("l2_weight", 1e-4, 0.1)
    filters = trial.suggest_int("filters", 5, 50)
    kernel_size = trial.suggest_categorical("kernel_size", [3, 5])
    depth_multiplier = trial.suggest_int("depth_multiplier", 1, 3)
    activation = trial.suggest_categorical("activation", ["sigmoid", "relu", "elu"])
    stacks = trial.suggest_int("stacks", 1, 4)
    lr = trial.suggest_loguniform("lr", 0.001, 0.1)
    batch_size = trial.suggest_int("batch_size", 10, 100)

    X = np.random.uniform(-2, 2, size=(10000, 10, 3))
    y = np.mean(np.mean(X[:, 1:, :] - X[:, :-1, :] > 0, axis=2) > 0.5, axis=1) > 0.5
    X += np.random.normal(0, 0.01, size=X.shape)
    train_X = X[1000:]
    train_y = y[1000:]
    test_X = X[:1000]
    test_y = y[:1000]
    epochs = 10

    model = create_model(
        input_shape=X.shape[1:],
        stacks=stacks,
        filters=filters,
        kernel_size=kernel_size,
        depth_multiplier=depth_multiplier,
        l2_weight=l2_weight,
        activation=activation,
    )
    model.compile(
        loss="binary_crossentropy",
        optimizer=keras.optimizers.Adam(lr),
        metrics=["accuracy"],
    )

    # It is generally a bad practice to set validation data to test data. However,
    # Optuna applies pruning by comparing training metric with previous observations
    # (which are test metric), so I velieve that the training metric should be the
    # same as the test metric.
    model.fit(
        train_X,
        train_y,
        batch_size=batch_size,
        validation_data=(test_X, test_y),
        epochs=epochs,
        verbose=0,
        callbacks=[Pruning(trial, "val_acc")],
    )
    test_loss, test_accuracy = model.evaluate(
        test_X, test_y, batch_size=batch_size, verbose=0
    )
    trial.set_user_attr("test_acc", test_accuracy)

    return test_accuracy


def create_model(
    input_shape, stacks, filters, kernel_size, depth_multiplier, l2_weight, activation
):
    x_input = L.Input(input_shape)
    net = x_input
    net = L.Conv1D(
        filters, kernel_size=kernel_size, activation="sigmoid", padding="same"
    )(net)
    net = L.BatchNormalization()(net)
    for _ in range(stacks):
        res = net
        net = L.SeparableConv1D(
            filters,
            kernel_size=kernel_size,
            depth_multiplier=depth_multiplier,
            kernel_regularizer=keras.regularizers.l2(l2_weight),
            activation=activation,
            padding="same",
        )(net)
        net = LayerNormalization()(net)
        net = L.add([res, net])
    net = L.GlobalAveragePooling1D()(net)
    y_output = L.Dense(1, activation="sigmoid")(net)
    return keras.Model(x_input, y_output)


class LayerNormalization(L.Layer):
    def build(self, input_shape):
        shape = (input_shape[-1],)
        self.gamma = self.add_weight(shape=shape, initializer="ones", name="gamma")
        self.beta = self.add_weight(shape=shape, initializer="zeros", name="beta")
        self.var_epsilon = K.epsilon() ** 2
        super().build(input_shape)

    def call(self, x):
        mean = K.mean(x, axis=-1, keepdims=True)
        var = K.var(x, axis=-1, keepdims=True)
        output = (x - mean) / K.sqrt(var + self.var_epsilon)
        output = self.gamma * output + self.beta
        return output


class Pruning(keras.callbacks.Callback):
    def __init__(self, trial, monitor):
        super().__init__()
        self.trial = trial
        self.monitor = monitor

    def on_epoch_end(self, epoch, logs):
        self.trial.report(logs[self.monitor], step=epoch)
        if self.trial.should_prune(epoch):
            raise optuna.structs.TrialPruned(f"{epoch}")


if __name__ == "__main__":
    main()
