#!/bin/sh
#
# @file test_while.sh
# @author Akagi201
# @date 2014/05/12
#

# while 语法结构
# while argument; do
#	statement
#	....
#done

# 无限循环使用的几种写法

#while ((1)); do
#	echo "hello"
#done

#while [ 1 ]; do
#	echo "hello"
#done

#while :; do
#	echo "hello"
#done

# 计算1到10的和

declare -i i=1
declare -i sum=0
while ((i <= 10)); do
	let sum+=i
	let ++i
done
echo $sum


