# MNIST
#
# Conv embedding -> positional encoding -> TransformerEncoder
#

import numpy as np
import sklearn.datasets
import sklearn.model_selection
import sklearn.preprocessing
import skorch
import torch
import torch.nn as nn
import torch.nn.functional as F


def main():
    random = np.random.RandomState(0)
    torch.manual_seed(random.randint(1e10))

    # Load MNIST
    mnist = sklearn.datasets.fetch_openml("mnist_784")

    X = mnist.data
    X = X.reshape((X.shape[0], 1, 28, 28)).astype(np.float32)
    X = np.log2(1 + X) / 8

    label_encoder = sklearn.preprocessing.LabelEncoder()
    y = label_encoder.fit_transform(mnist.target.reshape(-1, 1))

    X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(
        X, y, test_size=0.2, random_state=random,
    )

    print("Train:", X_train.shape, y_train.shape)
    print("Test: ", X_test.shape, y_test.shape)

    # Model
    module_options = {
        "module": ConvAttention,
        "module__width": X.shape[2],
        "module__height": X.shape[3],
        "module__input_dim": X.shape[1],
        "module__output_dim": len(label_encoder.classes_),
    }

    loss_options = {
        "criterion": nn.NLLLoss,
    }

    iterator_options = {
        "iterator_train__shuffle": True,
    }

    train_options = {
        "batch_size": 128,
        "max_epochs": 30,
        "device": "cuda",
    }

    model = skorch.NeuralNetClassifier(
        **module_options, **loss_options, **iterator_options, **train_options
    )
    model.fit(X_train, y_train)

    print("Test accuracy:", model.score(X_test, y_test))


class ConvAttention(nn.Module):
    def __init__(self, width, height, input_dim, output_dim):
        super().__init__()

        hidden_dim = 50
        bneck_dim = 10
        num_heads = 5
        num_transformer = 3

        self.conv1 = nn.Conv2d(input_dim, hidden_dim, 3)
        self.norm1 = nn.BatchNorm2d(hidden_dim)
        self.drop1 = nn.Dropout(0.5)
        self.conv2 = nn.Conv2d(hidden_dim, hidden_dim, 3)
        self.norm2 = nn.BatchNorm2d(hidden_dim)
        self.pool = nn.MaxPool2d(2)

        position = make_positional_encoding_2d(hidden_dim, (width - 4) // 2, (height - 4) // 2)
        position = position.astype(np.float32)
        self._position = torch.from_numpy(position).to(torch.device("cuda"))

        self.transformer = nn.TransformerEncoder(
            nn.TransformerEncoderLayer(hidden_dim, num_heads, dim_feedforward=bneck_dim),
            num_transformer
        )
        self.fc = nn.Linear(hidden_dim, output_dim)

    def forward(self, x):
        x = self.conv1(x)
        x = self.norm1(x)
        x = F.relu(x)
        x = self.drop1(x)

        x = self.conv2(x)
        x = self.norm2(x)
        x = F.relu(x)
        x = self.pool(x)

        x = x + self._position
        x = torch.flatten(x, start_dim=2)  # (batch, channels, width*height)
        x = x.permute(0, 2, 1)  # (batch, position, channels)
        x = self.transformer(x)

        # Global average pooling
        x = torch.mean(x, dim=1)

        x = self.fc(x)
        x = F.softmax(x, dim=1)

        return x


def make_positional_encoding_2d(channels, width, height):
    x = np.arange(width)
    y = np.arange(height)
    k = np.linspace(0, 5, num=((channels + 1) // 2))
    freq_x = 11 * np.pi * np.exp(-k)
    freq_y = 12 * np.pi * np.exp(-k)
    phase = freq_x[:, None, None] * x[None, :, None] + freq_y[:, None, None] * y[None, None, :]
    position = np.concatenate([np.cos(phase), np.sin(phase)])
    return position[:channels, :, :]


main()
