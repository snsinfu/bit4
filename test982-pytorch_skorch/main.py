import numpy as np
import sklearn.datasets
import sklearn.model_selection
import sklearn.preprocessing
import skorch
import torch
import torch.nn as nn
import torch.nn.functional as F


mnist = sklearn.datasets.fetch_openml("mnist_784")


class CNN(nn.Module):
    def __init__(self, input_dim, output_dim):
        super().__init__()
        hidden_dims = [5, 20, 30, 40, 20]

        self.conv0 = nn.Conv2d(input_dim, hidden_dims[0], 1)

        self.conv1 = nn.Conv2d(hidden_dims[0], hidden_dims[1], 3, padding=1)
        self.norm1 = nn.BatchNorm2d(hidden_dims[1])
        self.drop1 = nn.Dropout(0.5)

        self.conv2 = nn.Conv2d(hidden_dims[1], hidden_dims[2], 5, padding=2)
        self.norm2 = nn.BatchNorm2d(hidden_dims[2])

        self.conv3 = nn.Conv2d(hidden_dims[2], hidden_dims[3], 3, padding=1)
        self.norm3 = nn.BatchNorm2d(hidden_dims[3])

        self.conv4 = nn.Conv2d(hidden_dims[3], hidden_dims[4], 3)
        self.norm4 = nn.BatchNorm2d(hidden_dims[4])

        self.fc = nn.Linear(hidden_dims[4], output_dim)

    def forward(self, x):
        x = self.conv0(x)

        x = self.conv1(x)
        x = self.norm1(x)
        x = F.relu(x)
        x = F.max_pool2d(x, 2)
        x = self.drop1(x)

        x = self.conv2(x)
        x = self.norm2(x)
        x = F.relu(x)
        x = F.max_pool2d(x, 2)

        x = self.conv3(x)
        x = self.norm3(x)
        x = F.relu(x)
        x = F.max_pool2d(x, 2)

        x = self.conv4(x)
        x = self.norm4(x)
        x = F.relu(x)
        x = torch.mean(x, dim=[2, 3])

        x = self.fc(x)
        x = F.softmax(x, 1)

        return x



random = np.random.RandomState(0)
label_encoder = sklearn.preprocessing.LabelEncoder()

X = mnist.data
X = X.reshape((X.shape[0], 1, 28, 28)).astype(np.float32)
X = np.log2(1 + X) / 8
# X /= 255

y = label_encoder.fit_transform(mnist.target.reshape(-1, 1))

X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(
    X, y, test_size=0.2, random_state=random,
)

print("Train:", X_train.shape, y_train.shape)
print("Test: ", X_test.shape, y_test.shape)


module_options = {
    "module": CNN,
    "module__input_dim": 1,
    "module__output_dim": len(label_encoder.classes_),
}

loss_options = {
    "criterion": nn.NLLLoss,
}

iterator_options = {
    "iterator_train__shuffle": True,
}

train_options = {
    "batch_size": 512,
    "max_epochs": 20,
    "device": "cuda",
}

cnn = skorch.NeuralNetClassifier(
    **module_options, **loss_options, **iterator_options, **train_options
)
cnn.fit(X_train, y_train)
