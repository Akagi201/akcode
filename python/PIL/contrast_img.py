#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Image as im
import ImageEnhance as ie
img = im.open("p1.jpg")

contrast = ie.Contrast(img)
contrast_img = contrast.enhance(2.0)
contrast_img.save("contrast.jpg")
