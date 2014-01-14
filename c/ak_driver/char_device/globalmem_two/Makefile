
###############################################################################
# @file Makefile
# @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
# @brief 交叉编译 Makefile
# 
# @author   liuboyf1
# @data     2012-08-30
# @version  V1.0.0
###############################################################################

# Comment/uncomment the following line to enable/disable debugging
#DEBUG = y
ifeq ($(DEBUG),y)
	DEBFLAGS = -O -g # "-O" is needed to expand inlines
	DEBFLAGS += -DDEBUG_PRINTK # 控制是否打印信息
	DEBFLAGS += -DDEBUG_PROC #控制是否加载proc接口
	DEBFLAGS += -DDEBUG_WITH_LINE # 控制是否需要详细打印代码所在的行、位置
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

MODULE_NAME = globalmem_two
ifneq ($(KERNELRELEASE),)


obj-m := $(MODULE_NAME).o
#$(MODULE_NAME)-objs := file_opr.o mem_proc.o main.o

else

# in my Debian6 2.6.32
KERNELDIR ?=/lib/modules/2.6.32/build
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
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.* .*.o.cmd .*.ko.cmd .tmp_versions Module.symvers modules.order

.PHONY: modules modules_install clean

depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif
