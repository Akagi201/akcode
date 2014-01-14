#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
演示一维数组的下标存取
"""
import numpy as np

a = np.arange(10)
print "a", a 

print "a[5]", a[5]           # 用整数作为下标可以获取数组中的某个元素
print "a[3:5]", a[3:5]       # 用切片作为下标获取数组的一部分，包括a[3]不包括a[5]
print "a[:5]", a[:5]         # 切片中省略开始下标，表示从a[0]开始
print "a[:-1]", a[:-1]       # 下标可以使用负数，表示从数组最后往前数
a[2:4] = 100, 101            # 下标还可以用来修改元素的值
print "a", a 
print "a[1:-1:2]", a[1:-1:2] # 切片中的第三个参数表示步长，2表示隔一个元素取一个元素
print "a[::-1]", a[::-1]     # 省略切片的开始下标和结束下标，步长为-1，整个数组头尾颠倒
print "a[5:1:-2]", a[5:1:-2] # 步长为负数时，开始下标必须大于结束下标

b = a[3:7]                   # 通过切片产生一个新的数组b，b和a共享同一块数据存储空间
print "b", b
b[2] = -10
print "b", b                 # 将b的第2个元素修改为-10
print "a", a                 # a的第5个元素也被修改为10

x = np.arange(10, 1, -1)
print "x", x
print "x[[3, 3, 1, 8]]", x[[3, 3, 1, 8]] # 获取x中的下标为3, 3, 1, 8的4个元素，组成一个新的数组
b = x[np.array([3, 3, -3, 8])]           # 下标可以是负数
b[2] = 100
print "b", b
print "x", x                             # 由于b和x不共享数据空间，因此x中的值并没有改变
x[[3, 5, 1]] = -1, -2, -3                # 整数序列下标也可以用来修改元素的值
print x

x = np.arange(5, 0, -1)
print "x", x
## 布尔数组中下标为0，2的元素为True，因此获取x中下标为0,2的元素
print x[np.array([True, False, True, False, False])]
## 如果是布尔列表，则把True当作1, False当作0，按照整数序列方式获取x中的元素
print x[[True, False, True, False, False]]
## 布尔数组的长度不够时，不够的部分都当作False
print x[np.array([True, False, True, True])]
## 布尔数组下标也可以用来修改元素
x[np.array([True, False, True, True])] = -1, -2, -3
print "x", x

x = np.random.rand(10) # 产生一个长度为10，元素值为0到1的随机数组
print "x", x 
print x > 0.5 # 数组x中的每个元素和0.5进行大小比较，得到一个布尔数组，True表示x中对应的值大于0.5
print x[x > 0.5] # 使用x>0.5所得到的布尔数组收集x中的元素，因此结果就是包含x中所有大于0.5的元素的数组
