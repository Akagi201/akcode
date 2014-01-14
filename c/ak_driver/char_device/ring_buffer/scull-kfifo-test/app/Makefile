
CROSS_COMPILE	=/home/tekkaman/working/crosstool-gcc410-k26222/gcc-4.1.0-glibc-2.3.2/arm-9tdmi-linux-gnu/bin/arm-9tdmi-linux-gnu-
CC	= $(CROSS_COMPILE)gcc


all : scull_kfifo_test.o
		$(CC)  -o scull_kfifo_test scull_kfifo_test.o 
install : 
	cp scull_kfifo_test  /home/tekkaman/working/rootfs/tmp/

clean:
		rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions




