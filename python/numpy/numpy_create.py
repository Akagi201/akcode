#!/usr/bin/env python
# -*- coding: utf-8 -*-

## NumPy提供了很多专门用于创建数组的函数

"""
arange()类似于内置函数range()，通过指定开始值、终值和步长创建表示等差数列的一维数组，注意所得到的结果数组不包含终值。
例如下面的程序创建开始值为0、终值为1、步长为0.1的等差数组，注意终值1不在数组中：
"""
import numpy as np

print np.arange(0, 1, 0.1)

"""
linspace()通过指定开始值、终值和元素个数创建表示等差数列的一维数组，可以通过endpoint参数指定是否包含终值，缺省值为True，即包含终值。
下面两个例子分别演示了endpoint为True和False时的结果，注意endpoint的值会改变数组的等差步长：
"""
print np.linspace(0, 1, 10) # 步长为 1/9
print np.linspace(0, 1, 10, endpoint = False) # 步长为 1/10

"""
logspace()和linspace()类似，不过它所创建的数组是等比数列。
下面的例子产生从到、有5个元素的等比数列，注意起始值0表示，而终值2表示：
"""
print np.logspace(0, 2, 5)

"""
基数可以通过base参数指定，其缺省值为10。
下面通过将base参数设置为2，并设置endpoint参数为True，创建一个比例为2^(1/12)的等比数组
"""
print np.logspace(0, 1, 12, base = 2, endpoint = False)

"""
zeros()、ones()、empty()可以创建指定形状和类型的数组。其中empty()只分配数组所使用的内存，不对数组元素进行初始化操作，
因此它的运行速度是最快的。下面的程序创建一个形状为(2,3)，元素类型为整数的数组：
"""
print np.empty((2, 3), np.int)  # 只分配内存, 不对其进行初始化

"""
而zeros()则将数组元素初始化为0，ones()将数组元素初始化为1。
下面创建一个长度为4、元素类型为浮点数的一维数组，并且元素全部初始化为0：
"""
print np.zeros(4, np.float)
print np.ones(4, np.float)

"""
使用frombuffer()、fromstring()、fromfile()等函数可以从字节序列或者文件创建数组
"""
s = "abcdefgh"

"""
Python的字符串实际上是一个字节序列，每个字符占一个字节，因此如果从字符串s创建一个8bit的整数数组，
所得到的数组正好就是字符串中每个字符的ASCII编码
"""
print np.fromstring(s, dtype = np.int8)

"""
如果从字符串s创建16bit的整数数组，那么两个相邻的字节就表示一个整数，把字节98和字节97当作一个16位的整数，
它的值就是98*256+97 = 25185。可以看出16bit的整数是以低位字节在前(little-endian)的方式保存在内存中的
"""
print np.fromstring(s, dtype = np.int16)

## 把整个字符串转换为一个64bit的双精度浮点数数组
print np.fromstring(s, dtype = np.float)

## 可以先定义一个从下标计算数值的函数，然后用fromfunction()通过此函数创建数组
def func(i):
	return i % 4 + 1
"""
fromfunction()的第一个参数为计算每个数组元素的函数，第二个参数指定数组的形状。因为它支持多维数组，
所以第二个参数必须是一个序列。上例中第二个参数是长度为1的元组(10,)，因此创建了一个有10个元素的一维数组。
"""
np.fromfunction(func, (10,))

## 下面的例子创建一个表示九九乘法表的二维数组，输出的数组a中的每个元素a[i, j]都等于func2(i, j)：
def func2(i, j):
	return (i + 1) * (j + 1)

a = np.fromfunction(func2, (9, 9))
print a
