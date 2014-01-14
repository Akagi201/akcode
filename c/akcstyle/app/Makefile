
###############################################################################
# @file     Makefile
# @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
# @brief    Linux Application Generic Makefile
# 
# @author   liuboyf1
# @date     2012-09-28
# @version  V1.0.6
# @note     History:
# @note     <author>    <time>       <version>    <desc>
# @note     liuboyf1    2012-09-03   V1.0.0       创建文件
# @note     liuboyf1    2012-09-26   V1.0.1       修复了DEBFLAGS调试
# @note     liuboyf1    2012-09-28   V1.0.2       修改rm为-rm更健壮
# @note     liuboyf1    2012-09-28   V1.0.3       修改了部分注释
# @note     liuboyf1    2012-10-09   V1.0.4       修改了rm注释不能在同一行
# @note     liuboyf1    2012-10-15   V1.0.5       修改了调试参数和布局
# @note     liuboyf1    2012-10-18   V1.0.6       修改了TOOL_DIR和TOOL_PREFIX
###############################################################################

# The prefix to be added before the GNU compiler tools (optionally including
# path), i.e. "arm-linux-" or "/opt/bin/arm-linux-".
#TOOL_DIR=$(shell if [ -e /home/v5t_le-mv401_uc ]; then echo /home; else echo /opt; fi)
# 交叉编译工具
#TOOL_PREFIX=$(TOOL_DIR)/v5t_le-mv401_uc/bin/arm_v5t_le-

# Comment/uncomment the following line to enable/disable debugging
#DEBUG = y
ifeq ($(DEBUG),y)
	DEBFLAGS = -O -g # "-O" is needed to expand inlines
	#DEBFLAGS += -DDEBUG # 控制是否打印调试和错误信息
else
	DEBFLAGS = -O2
endif

CFLAGS += -Wall
#INCLUDES = -I/xxx 
#LIBS = -lpthread

#tool chain
CC = $(TOOL_PREFIX)gcc

SRCS = $(wildcard *.c) # 当前目录下所有以.c结尾的源文件,wildcard 扩展通配符
OBJS = $(SRCS:.c = .o) # .c=.o是做一个替换,把变量$(sources)所有[.c]的字串都替换成.o
# OBJS = $(patsubst %.c, %.o, $(SRCS))

PWD := $(shell pwd)

# 生成的可执行文件名称
TARGET = test

$(TARGET) : $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) $(DEBFLAGS) $(INCLUDES) $(LIBS)
	#cp $(TARGET) $(PWD)/..

%.o : %.c
	$(CC) -c $< $(CFLAGS) $(DEBFLAGS)

clean :
# 忽略某些文件问题，继续做后面的事情
	-rm -f *.o $(TARGET)

.PHONY : clean
