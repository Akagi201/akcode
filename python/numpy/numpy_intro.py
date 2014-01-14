#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
演示NumPy数组的一些基本功能
"""
import numpy as np

## 下面的例子都是先创建一个Python的序列对象，然后通过array()将其转换为数组，这样做显然效率不高。
a = np.array([1, 2, 3, 4])
b = np.array((5, 6, 7, 8))
c = np.array([[1, 2, 3, 4], [4, 5, 6, 7], [7, 8, 9, 10]])

print a
print b
print c

# 数组的形状可以通过其shape属性获得，它是一个描述数组各个轴长度的元组(tuple)：
c.shape = 4, 3
print c

c.shape = 2, -1
print c

## 数组a和d其实共享数据存储空间，因此修改其中任意一个数组的元素都会同时修改另外一个数组的内容：
d = a.reshape((2, 2))
print d

a[1] = 100
print d

# 数组的元素类型可以通过dtype属性获得。
print c.dtype

e = np.array([
	[1, 2, 3, 4],
	[4, 5, 6, 7],
	[7, 8, 9, 10]], dtype = np.float)

print e
print e.dtype

f = np.array([1, 2, 3, 4], dtype = np.float)
g = np.array([1, 2, 3, 4], dtype = np.complex)

print f
print f.dtype

print g
print g.dtype

## NumPy中的数据类型都有几种字符串表示方式，字符串和类型之间的对应关系都储存在typeDict字典中，例如’d’、 ‘double’、’float64’都表示双精度浮点数类型：
print np.typeDict["d"]
print np.typeDict["double"]
print np.typeDict["float64"]

## 完整的类型列表可以通过下面的语句得到，它将typeDict字典中所有的值转换为一个集合，从而去除其中重复项：
print set(np.typeDict.values())