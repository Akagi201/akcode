#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Image as im
import ImageEnhance as ie
img = im.open("p1.jpg")
sharpness = ie.Sharpness(img)
sharp_img = sharpness.enhance(7.0)
sharp_img.save("sharp.jpg")