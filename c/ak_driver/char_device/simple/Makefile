
###############################################################################
# @file Makefile
# @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
# @brief Top Generic Mkefile
# 
# @author   liuboyf1
# @data     2012-09-26
# @version  V1.0.0
# @note     History:
# @note     <author>    <time>       <version>    <desc>
# @note     liuboyf1    2012-09-11   V1.0.0       创建文件
# @note     liuboyf1    2012-09-26   V1.0.1       添加make app make dev分别编译功能
###############################################################################

PWD = $(shell pwd)
all: app dev
app:
	make -C $(PWD)/app
dev:
	make -C $(PWD)/dev
clean:
	make clean -C $(PWD)/app
	make clean -C $(PWD)/dev
.PHONY: all app dev clean
