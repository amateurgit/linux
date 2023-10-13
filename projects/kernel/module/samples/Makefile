obj-m += hello.o
KERNEL_DIR:=/lib/modules/$(shell uname -r)/build
MODULEDIR:=$(shell pwd)

modules:
	$(MAKE) -C $(KERNEL_DIR) M=$(MODULEDIR) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(MODULEDIR) modules clean
