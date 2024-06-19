import os
import torch.nn as nn
import torch
from torch import optim
from torch.utils.data import DataLoader
from data import MyDataset
from unet import UNet

# GPU和CPU的选择
# device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
device = torch.device('cpu')


weight_path = './params/unet.pth'
data_path = './Datasets/train'
input_channel = 2

# 确保保存路径存在
os.makedirs(os.path.dirname(weight_path), exist_ok=True)


def train_epoch(data_loader, model, loss_function, optimizer, device, accumulation_steps=4):
    model.train()
    optimizer.zero_grad()
    for i, (input_image, label_image) in enumerate(data_loader):

        input_image, label_image = input_image.to(device), label_image.to(device)

        out_image = model(input_image)
        train_loss = loss_function(out_image, label_image) / accumulation_steps

        train_loss.backward()

        if (i + 1) % accumulation_steps == 0:
            optimizer.step()
            optimizer.zero_grad()

        print(f'Batch {i}, Train Loss: {train_loss.item() * accumulation_steps}')

        # 清理 GPU 内存缓存
        torch.cuda.empty_cache()


if __name__ == '__main__':
    data_loader = DataLoader(MyDataset(data_path), batch_size=2, shuffle=False)
    net = UNet(input_channel).to(device)

    if os.path.exists(weight_path):
        net.load_state_dict(torch.load(weight_path, map_location=device))
        print('Successful load weight!')
    else:
        print('Not successful load weight')

    opt = optim.Adam(net.parameters())
    loss_fun = nn.MSELoss()

    epoch = 1
    while epoch <= 100:
        train_epoch(data_loader, net, loss_fun, opt, device)

        if epoch % 5 == 0:
            print(f'{epoch} epochs completed. Saving model...')
            torch.save(net.state_dict(), weight_path + str(epoch))

        epoch += 1
