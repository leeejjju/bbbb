obj-m = hello.o
KDIR = /lib/module/$(shell uname -r)/build
PWD = $(shell pwd)
all: module
module:
	$(MAKE) -C M=$(PWD) modules
clean:
	$(MAKE) -C M=$(PWD) clean
	rm -rf Module.sysvers modules.order
