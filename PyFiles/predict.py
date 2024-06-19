import os
import sys
import numpy as np
from PIL import Image
import torch
from torch.utils.data import DataLoader
from data import MyDataset
from unet import UNet
from torchvision import transforms

# 加载测试集
path = './Datasets/test'

# 保存路径
save_path = './predict_results/'

# 定义 transform
transform = transforms.Compose([
    transforms.ToTensor()
])

# 加载训练好的模型
weights = './params/unet.pth'
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')


def readPng(filepath, bandnumber=1):
    # 打开图像文件
    img = Image.open(filepath)

    # 将图像转换为 numpy 数组
    im_datas = np.array(img)

    # 检查图像是否是多通道图像（比如 RGB 或 RGBA）
    if len(im_datas.shape) == 3:
        # 提取指定波段的数据，Pillow 中的通道索引从0开始
        im_datas = im_datas[:, :, bandnumber - 1]

    return im_datas


def load_image(image1_path, image2_path):
    image1 = readPng(image1_path)
    image2 = readPng(image2_path)

    # 合并变化前和变化后的图像
    input_image = np.stack([image1, image2], axis=2)
    # 因为input_image是三维的，所以label_image需要增加一个维度
    return transform(input_image).unsqueeze(0).type(torch.float32)  # Add

    # return transform(input_image).type(torch.float32), \
    #        (torch.unsqueeze(torch.from_numpy(label_image), dim=0) / 255).type(torch.float32)


input_channel = 2
net = UNet(input_channel).to(device)
net.eval()
load_models = torch.load(weights)

if os.path.exists(weights):
    net.load_state_dict(torch.load(weights))
    print('successfully')
else:
    print('no loading')


def main(before_image_path, after_image_path, output_image_path):
    input_image = load_image(before_image_path, after_image_path).to(device)

    with torch.no_grad():
        out_image = net(input_image)
        _out_image = out_image[0][0].round().detach().cpu().numpy()
        _out_image = (_out_image * 255).astype(np.uint8)

        result_image = Image.fromarray(_out_image)
        result_image.save(output_image_path)


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print(f'Usage: {sys.argv[0]} <before_image_path> <after_image_path> <output_image_path>')
        sys.exit(1)

    before_image_path = sys.argv[1]
    after_image_path = sys.argv[2]
    output_image_path = sys.argv[3]
    main(before_image_path, after_image_path, output_image_path)












