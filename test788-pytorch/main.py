import numpy as np
import torch
import torch.nn as nn
import torch.utils.data


def main():
    model = Model(2)

    # PyTorch uses (batch, channel, timesteps) convention.
    x = np.random.uniform(-1, 1, size=(10000, 2, 100))
    a = np.mean(x[:, 0, :50] + x[:, 1, 50:], axis=1)
    b = np.mean(x[:, 1, :50] + x[:, 0, 50:], axis=1)
    y = np.reshape(a < b, (-1, 1))

    valid_size = 1000
    batch_size = 100

    train_x = x[valid_size:]
    train_y = y[valid_size:]
    valid_x = x[:valid_size]
    valid_y = y[:valid_size]

    train_dataset = torch.utils.data.TensorDataset(
        torch.Tensor(train_x), torch.Tensor(train_y)
    )
    train_loader = torch.utils.data.DataLoader(
        train_dataset, batch_size=batch_size, shuffle=True
    )

    valid_dataset = torch.utils.data.TensorDataset(
        torch.Tensor(valid_x), torch.Tensor(valid_y)
    )
    valid_loader = torch.utils.data.DataLoader(train_dataset, batch_size=batch_size)

    optimizer = torch.optim.Adam(model.parameters(), lr=0.005)
    criterion = nn.BCELoss()

    for epoch in range(10):
        model.train()

        avg_loss = 0
        for x_batch, y_batch in train_loader:
            optimizer.zero_grad()
            y_pred = model(x_batch)
            loss = criterion(y_pred, y_batch)
            loss.backward()
            optimizer.step()
            avg_loss += loss.item()
        avg_loss /= len(train_loader)

        model.eval()

        val_loss = 0
        for x_batch, y_batch in valid_loader:
            y_pred = model(x_batch)
            loss = criterion(y_pred, y_batch)
            val_loss += loss.item()
        val_loss /= len(valid_loader)

        print(f"{epoch + 1}\ttrain {avg_loss:.3g}\tval {val_loss:.3g}")


class Model(nn.Module):
    def __init__(self, input_dim):
        super().__init__()
        self.front = nn.Conv1d(input_dim, 10, kernel_size=1)
        self.tcn = TCN(10, kernel_size=5, dilations=[1, 2, 4, 8, 16])
        self.output = nn.Linear(10, 1)

    def forward(self, x):
        y = self.front(x)
        y = self.tcn(y)
        y = torch.mean(y, 2)  # Avg pool over timesteps
        y = self.output(y)
        y = torch.sigmoid(y)
        return y


class TCN(nn.Module):
    def __init__(self, channels, kernel_size, dilations):
        super().__init__()
        self.steps = nn.ModuleList()
        for d in dilations:
            self.steps.append(TCNStep(channels, kernel_size, d))

    def forward(self, x):
        y = x
        for step in self.steps:
            y = y + step(y)
        return y


class TCNStep(nn.Module):
    def __init__(self, channels, kernel_size, dilation):
        super().__init__()
        self.conv = nn.Conv1d(
            channels,
            channels,
            kernel_size,
            padding=(dilation * (kernel_size - 1) // 2),
            dilation=dilation,
        )
        self.act = nn.ELU()
        self.norm = nn.BatchNorm1d(channels)

    def forward(self, x):
        y = self.conv(x)
        y = self.act(y)
        y = self.norm(y)
        return y


if __name__ == "__main__":
    main()
