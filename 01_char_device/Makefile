ifneq ($(KERNELRELEASE),)
	obj-m := vchar_driver.o
else
	KERNELDIR := /home/chen/qemu_linux_code/linux-5.15.18
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
endif