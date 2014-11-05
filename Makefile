obj-m += kl.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	modinfo kl.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

