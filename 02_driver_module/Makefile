KDIR := /home/chen/qemu_linux_code/linux-5.15.18

PWD := $(shell pwd)

obj-m := hello.o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean