
###############################################################################
# @file Makefile
# @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
# @brief Top Makefile
# 
# @author   liuboyf1
# @data     2012-09-11
# @version  V1.0.0
###############################################################################

PWD = $(shell pwd)
all:
	make -C $(PWD)/server
	make -C $(PWD)/client
clean:
	make clean -C $(PWD)/server
	make clean -C $(PWD)/client
.PHONY: all clean
