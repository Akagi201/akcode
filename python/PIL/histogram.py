#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Image as im
img = im.open('p1.jpg')
res_img = img.resize((128,128), im.BILINEAR)
rot_img = res_img.rotate(45)
print rot_img.histogram()