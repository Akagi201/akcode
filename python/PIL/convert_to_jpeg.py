#!/usr/bin/env python
# -*- coding: utf-8 -*-
## 将文件转换成JPEG

import os, sys
import Image

for infile in sys.argv[1:]:
    f, e = os.path.splitext(infile)
    outfile = f + ".jpg"
    if infile != outfile:
        try:
            Image.open(infile).save(outfile)
        except IOError:
            print "cannot convert", infile
