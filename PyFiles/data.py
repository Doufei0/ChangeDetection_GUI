import os
import torch
from torch.utils.data import Dataset
import numpy as np
from torchvision import transforms
from PIL import Image
import numpy as np

transform = transforms.Compose([
    transforms.ToTensor()
])


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


# 所有定义的dataset类都需要继承Dataset
# 使用self关键字的目的是将变量绑定到类的实例上。在类的方法中，使用self可以访问该类的实例变量和方法
class MyDataset(Dataset):
    def __init__(self, path):
        self.path = path
        # 获取path目录下label子目录中的所有文件名，并将它们存储在self.name属性中
        self.name = os.listdir(os.path.join(path, 'label'))  # 对lable文件夹做list

    # 用于返回数据集的长度，即数据样本的数量
    def __len__(self):
        return len(self.name)

    # python中 函数的定义、类的定义、for循环
    def __getitem__(self, index):
        # 对于train_a.png,image.split('_')的结果为“train a.png”
        # 读标签图像：读之前要获取索引的路径
        label_name = [image for image in os.listdir(os.path.join(self.path, 'label'))
                      if image.split('_')[1] == str(index + 1) + '.png'][0]  # [0]表示有且仅有一个元素与索引的结果相对应
        label_path = os.path.join(self.path, 'label', label_name)  # 具体到索引的label

        label_image = readPng(label_path)
        h, w = label_image.shape

        #  输入变化前 image for image in（这种形式只有名称，但不包含路径）
        input_path1 = [os.path.join(self.path, 'A', image) for image in
                       os.listdir(os.path.join(self.path, 'A')) if image.split('_')[1] == str(index + 1) + '.png'][0]
        #  第一张与第二张已经使用了
        input_image1 = readPng(input_path1)

        #  输入变化后
        input_path2 = [os.path.join(self.path, 'B', image) for image in
                       os.listdir(os.path.join(self.path, 'B')) if image.split('_')[1] == str(index + 1) + '.png'][0]
        #  第一张与第二张已经使用了
        input_image2 = readPng(input_path2)

        #  汇总变化前和变化后
        # 表示在第三个维度上进行堆叠，那么堆叠后的input_image的形状将变为(height, width, 2)
        input_image = np.stack([input_image1, input_image2], 2)

        # 因为input_image是三维的，所以label_image需要增加一个维度
        return transform(input_image).type(torch.float32), \
               (torch.unsqueeze(torch.from_numpy(label_image), dim=0) / 255).type(torch.float32)
        # return transform(input_image).unsqueeze(0).type(torch.float32)  # Add


if __name__ == '__main__':
    data = MyDataset('./Datasets/train')

    input_img, label_img = data[0]
    print(input_img.shape)
    print(label_img.shape)
