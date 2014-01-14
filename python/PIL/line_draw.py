#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Image as im
import ImageDraw as imd
img = im.open("p1.jpg")
draw = imd.Draw(img)
width, height = img.size
draw.line(((0, 0), (width - 1, height - 1)), fill = 255)
draw.line(((0, height - 1), (width - 1, 0)), fill = 255)
img.save("cross_line.jpg")