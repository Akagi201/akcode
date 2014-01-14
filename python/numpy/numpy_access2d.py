#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
演示二维数组的下标存取。
"""
import numpy as np

a = np.arange(0, 60, 10).reshape(-1, 1) + np.arange(0, 6)
print "a", a
print "a[0,3:5]=", a[0,3:5]
print "a[4:,4:]=", a[4:,4:]
print "a[:,2]=", a[:,2]
print "a[2::2,::2]=", a[2::2,::2]
print "a[1::1,::1]=", a[1::1,::1]

"""
如果下标元组中只包含整数和切片，则所得到的数组和原始数组共享数据，它是原数组的视图
下面的例子中，数组b是a的视图，它们共享数据，因此修改b[0]时，数组a中对应的元素也被修改：
"""
b = a[0,3:5]
b[0] = -b[0]
print "a[0,3:5]=", a[0,3:5]
b[0] = -b[0]

## 因为数组的下标是一个元组，所以我们可以将下标元组保存起来，用同一个元组存取多个数组：
"""
slice对象
在[]中可以使用以冒号隔开的两个或者三个整数表示切片，但是单独生成切片对象时需要使用slice()创建。
它有三个参数，分别为开始值、结束值和间隔步长，当这些值需要省略时可以使用None。
例如，a[slice(None,None,None),2]和a[:,2]相同。
"""
idx = slice(None, None, 2), slice(2,None)
print "idx = ", idx
print "a[idx]=", a[idx] # 和a[::2,2:]相同
print "a[idx][idx]=", a[idx][idx] # 和a[::2,2:][::2,2:]相同
## 用Python的内置slice()函数创建下标比较麻烦，因此NumPy提供了一个s_对象帮助我们创建数组下标：
print np.s_[::2, 2:]

## 请注意s_实际上是一个IndexExpression类的对象：
print np.s_

print a[(0,1,2,3,4),(1,2,3,4,5)]
print a[3:, [0,2,5]]
mask = np.array([1,0,1,0,0,1], dtype=np.bool)
print a[mask, 2]
mask = np.array([1,0,1,0,0,1])
print a[mask, 2]
