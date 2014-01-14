#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Image as im
import ImageEnhance as ie
img = im.open("p1.jpg")
brightness = ie.Brightness(img)
bright_img = brightness.enhance(2.0)
bright_img.save("bright.jpg")

