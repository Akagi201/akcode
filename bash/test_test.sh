#!/bin/sh
#
# http://www.ibm.com/developerworks/cn/linux/l-bash-test.html
#
# @file test_test.sh
# @author Akagi201
# @date 2014/05/12
#

# test 与 [ ]
# 0: 真 / 1: 假
# test expr 与 [ expr ]等价, 注意空格

test 3 -gt 4 && echo true || echo false

[ "abc" != "def" ]; echo $?

test -d "$HOME"; echo $?

# -d: 检查它是不是目录

# 比较算术值
# -eq: 等于
# -ne: 不等于
# -lt: 小于
# -le: 小于等于
# -gt: 大于
# -ge: 大于等于

# 比较字符串
# =, !=, <, >
# -z: 测试null字符串
# -n: 测试字符串是否非空, 非空返回True

# 重定向, 需要转义
# < -> \<
# > -> \>

test "abc" = "def"; echo $?
[ "abc" != "def" ]; echo $?
[ "abc" \< "def" ]; echo $?
[ "abc" \> "def" ]; echo $?
[ "abc" \< "abc" ]; echo $?
[ "abc" \> "abc" ]; echo $?

# 文件测试, 如果被测试文件存在, 而且有指定的特征, 则结果为True
# 单目测试
# -d: 目录
# -e: 存在(也可以用-a)
# -f: 普通文件
# -h: 符号连接(也可以用-L)
# -p: 命名管道
# -r: 可读
# -s: 非空
# -S: 套接字
# -w: 可写
# -N: 从上次读取之后已经做过修改

# 双目测试
# -nt: 测试file1是否比file2更新, 修改日期将用于这次和下次比较.
# -ot: 测试file1是否比file2旧.
# -ef: 测试file1是不是file2的硬链接.
# man test, 查看更多选项




