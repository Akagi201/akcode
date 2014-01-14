#!/bin/bash

echo "The # here does not begin a comment."
echo 'The # here does not begin a comment.'
echo The \# here does not begin a comment.

echo ${PATH#*:} # 参数替换, 不是一个注释
echo $PATH
echo $(( 2#101011 )) # 数制转换, 不是一个注释

