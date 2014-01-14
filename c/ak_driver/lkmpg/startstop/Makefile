obj-m += startstop.o
startstop-objs := start.o stop.o

all:
	make -C /lib/modules/2.6.32/build M=$(PWD) modules
clean:
	make -C /lib/modules/2.6.32/build M=$(PWD) clean
