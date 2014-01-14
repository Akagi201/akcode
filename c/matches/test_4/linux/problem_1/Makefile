
###############################################################################
# @file Makefile
# @brief    Linux Device Driver Generic Makefile
# 
# @author   liuboyf1
# @data     2012-09-28
# @version  V1.0.5
# @note     History:
# @note     <author>    <time>       <version>    <desc>
# @note     liuboyf1    2012-08-30   V1.0.0       创建文件
# @note     liuboyf1    2012-09-26   V1.0.1       修改了CFLAGS为EXTRA_CFLAGS
# @note     liuboyf1    2012-09-26   V1.0.2       简化了调试宏部分
# @note     liuboyf1    2012-09-28   V1.0.3       修改rm为-rm
# @note     liuboyf1    2012-10-08   V1.0.4       修改make clean
# @note     liuboyf1    2012-10-10   V1.0.5       修改KERNELDIR
###############################################################################

# Comment/uncomment the following line to enable/disable debugging
#DEBUG = y
ifeq ($(DEBUG),y)
	DEBFLAGS = -O -g # "-O" is needed to expand inlines
	DEBFLAGS += -DDEBUG # 控制是否打印调试和错误信息
else
	DEBFLAGS = -O2
endif

# The prefix to be added before the GNU compiler tools (optionally including
# path), i.e. "arm-linux-" or "/opt/bin/arm-linux-".
#TOOL_DIR = /opt/v5t_le-mv401_uc
# 交叉编译工具
#TOOL_PREFIX = $(TOOL_DIR)/bin/arm_v5t_le-

CC = $(TOOL_PREFIX)gcc
#AR:= $(TOOL_PREFIX)ar -rv
EXTRA_CFLAGS += $(DEBFLAGS)
#EXTRA_CFLAGS += -I$(LDDINC)

MODULE_NAME = hello_printk
ifneq ($(KERNELRELEASE),)


obj-m := $(MODULE_NAME).o
#$(MODULE_NAME)-objs := file_opr.o mem_proc.o main.o

else

# in my Debian6 2.6.32
KERNELVER ?= $(shell uname -r)
KERNELDIR ?= /lib/modules/$(KERNELVER)/build
# in my IPC 2.6.18
#KERNELDIR ?= /home/akagi201/kernel_step

PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	cp $(MODULE_NAME).ko $(PWD)/..

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
endif

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
.PHONY: modules modules_install clean

depend .depend dep:
	$(CC) $(EXTRA_CFLAGS) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif
