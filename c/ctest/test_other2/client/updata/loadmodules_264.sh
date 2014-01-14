#!/bin/sh
#echo 2 > /proc/cpu/alignment
rm pidfile -rf
rm core* -rf

#ln -s /dev/mmcblk0 /dev/mmc0
#ln -s /dev/mmcblk0p1 /dev/mmc01
#ln -s /dev/mmcblk0p2 /dev/mmc02
#应用中使用的接到符号为rtc,而内核实际创建为rtc0
ln -s /dev/rtc0 /dev/rtc

rmmod cmemk 2>/dev/null
rmmod irqk 2>/dev/null
rmmod edmak 2>/dev/null
rmmod dm365mmap 2>/dev/null


echo 2 > /proc/sys/vm/overcommit_memory
echo 90 > /proc/sys/vm/overcommit_ratio

echo 1 > /proc/sys/vm/dirty_ratio
echo 1 > /proc/sys/vm/dirty_background_ratio
#脏数据驻留内存时间, 单位:1/100秒
echo 100 > /proc/sys/vm/dirty_expire_centisecs
#pdflush进程运行间隔时间, 单位:1/100秒
echo 50 > /proc/sys/vm/dirty_writeback_centisecs
echo 2048 > /proc/sys/vm/min_free_kbytes


# Pools configuration
#h264enc
#insmod cmemk.ko phys_start=0x86500000 phys_end=0x88000000 pools=2x2457600,1x1843200,1x4297168,1x100000,1x400000,1x300000,1x100000,3x100000,1x100000,2x100000,1x100000,1x100000,1x100000,1x100000,1x100000,1x100000,1x3550000,6x180,3x38016 allowOverlap=1 phys_start_1=0x00001000 phys_end_1=0x00008000 pools_1=1x28672
#---------------------------------------------------------------|misc            |<- for 2 * h264enc           -------------------------------------->|<-3 * ((UXGA + VGA) * 2)|MASK|1 x encoded buf| MD |
#insmod cmemk.ko phys_start=0x85800000 phys_end=0x88000000 pools=2x24,6x28,6x28,6x28,6x88,6x88,6x88,6x88,6x88,6x88,2x126,4x202,2x296,2x354,4x384,2x544,2x1088,2x1152,4x3200,2x4096,1x6144,2x7680,2x27840,2x30812,6x100000,1x1123584,3x3340864,1x3840000,1x4718592,3x6244352 allowOverlap=1 phys_start_1=0x00001000 phys_end_1=0x00008000 pools_1=1x28672
insmod cmemk.ko phys_start=0x85f00000 phys_end=0x88000000 pools=2x24,6x28,6x28,6x28,6x88,6x88,6x88,6x88,6x88,6x88,2x126,4x202,2x296,2x354,4x384,2x544,2x1088,2x1152,4x3200,2x4096,1x6144,2x7680,2x27840,2x30812,6x100000,1x1048576,1x1123584,3x3340864,1x3840000,1x4718592,2x6244352 allowOverlap=1 phys_start_1=0x00001000 phys_end_1=0x00008000 pools_1=1x28672

insmod irqk.ko 
insmod edmak.ko
insmod dm365mmap.ko

#/dev/dm365mmap节点创建较慢，故要等待30S
#sleep 30

./davinci &
/bin/pppoed &
