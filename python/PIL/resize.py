#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Image as im
img = im.open('p1.jpg')
res_img = img.resize((128,128), im.BILINEAR)
res_img.save("resize_img.jpg")
rot_img = res_img.rotate(45)
rot_img.save("rotate_img.jpg")
##  格式转换
rot_img.save("convert_img.bmp")