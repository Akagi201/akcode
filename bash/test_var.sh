#!/bin/bash
#
# @file test_var.sh
#
# @author Akagi201
# @date 2014/05/12
#

# location variables
echo '$1='"$1"

# $# 表示位置参数的数量
# $* 表示所有位置参数的内容
# $? 表示命令执行后返回的状态, 用于检查上一个命令的执行是否正确:
# 在linux中, 命令退出状态为0表示命令正确执行, 任何非0值表示命令
# 执行错误
# $$ 表示当前进程的进程号
# $! 表示后台运行的最后一个进程号
# $0 表示当前执行的进程名

# 自定义变量
DAY=sunday

# 自定义变量的查看与引用
echo $DAY

# 自定义变量的输出, 全局变量, 在子shell中也生效
export DAY

# 自定义变量的清除
unset DAY
