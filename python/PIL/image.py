#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Image as im

## 如果文件不能打开，会抛出一个IOError 异常。库会自动根据文件的内容来确定图像的格式。
img = im.open("lena.jpg")
"""
format 属性表示图像的原始格式。如果图像不是从文件中读取的，则它被设置成 None。
size 属性是一个2元组，表示图像的宽度和高度 （以像素为单位）。
mode 属性定义图像的色彩通道的数量与名字，同时也包括像素的类型和颜色深度信息。通常来说，灰度图像的mode是"L" (luminance)，真彩色图像的mode是 "RGB" ，而用来打印的图像的mode是"CMYK"。
"""
print img.format, img.size, img.mode

## show()方法把图像保存到磁盘中，并调用外部显示工具来显示它。
"""
show 的标准实现不是很高效，因为它先将图像保存成一个临时文件，然后调用 xv 程序来显示图像。如果你没有安装xv ，它甚至不能工作。然而如果它可用，它将是非常方便的出错和测试的工具。
"""
img.show()
