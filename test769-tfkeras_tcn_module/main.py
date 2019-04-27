import numpy as np
import tensorflow.keras as keras
import tensorflow.keras.layers as L

from nn.layers import LayerNormalization
from nn.modules import SymmetricTCN


model = keras.Sequential(
    [
        L.InputLayer((10, 1)),
        L.Conv1D(
            10,
            kernel_size=3,
            padding="same",
            activation="sigmoid",
            kernel_regularizer=keras.regularizers.l2(0.01),
        ),
        LayerNormalization(),
        SymmetricTCN(
            10,
            kernel_size=3,
            dilation_rates=[1, 2, 4],
            depth_multiplier=2,
            # dropout=0.5,
            # depthwise_regularizer=keras.regularizers.l2(0.01),
            # pointwise_regularizer=keras.regularizers.l2(0.01),
        ),
        L.GlobalAveragePooling1D(),
        L.Dense(1, activation="sigmoid"),
    ]
)
model.compile(
    loss="binary_crossentropy",
    optimizer=keras.optimizers.Adam(lr=0.05),
    metrics=["binary_crossentropy", "accuracy"],
)
print(model.summary())

X = np.random.uniform(-3, 3, size=(20000, 10, 1))
y = (X[:, :5, :].sum(axis=1) > X[:, 5:, :].sum(axis=1)) * 1.0
model.fit(X, y, batch_size=500, epochs=10, validation_split=0.1)


# kernel_regularizer            acc 0.9704 val_acc 0.9810       Good
# depthwise_regularizer         acc 0.9638 val_acc 0.9735       Good
# pointwise_regularizer         acc 0.9579 val_acc 0.9560       Mediocre
# kernel & depthwise            acc 0.9604 val_acc 0.9505       Bad
# all                           acc 0.9432 val_acc 0.9540       Mediocre
# dropout                       acc 0.9419 val_acc 0.9505       Mediocre
# kernel & dropout              acc 0.9387 val_acc 0.9480       Mediocre
# kernel & dropout 2x epoch     acc 0.9509 val_acc 0.9650       Good
