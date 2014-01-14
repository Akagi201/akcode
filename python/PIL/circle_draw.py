#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Image as im
import ImageDraw as imd
img = im.open("p1.jpg")
width, height = img.size
draw = imd.Draw(img)
draw.arc((0, 0, width - 1, height - 1), 0, 360, fill = 255)
img.save("circle.jpg")