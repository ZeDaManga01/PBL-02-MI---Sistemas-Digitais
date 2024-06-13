# Device name
DEVICE_NAME := colenda_driver

# Module name - "colenda_driver"
obj-m := $(DEVICE_NAME).o

# Kernel directory
KDIR := /lib/modules/$(shell uname -r)/build

# Current directory
PWD := $(shell pwd)

# Default rule to build the module
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Rule to clean up generated files
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

# Rule to install the module
install: all
	sudo cp $(DEVICE_NAME).ko /lib/modules/$(shell uname -r)/kernel/drivers/misc/
	sudo depmod -a
	sudo modprobe $(DEVICE_NAME)
	dmesg | tail -n 1

# Rule to uninstall the module
uninstall:
	rm -f $(DEVICE_NAME).mod.c $(DEVICE_NAME).mod.o $(DEVICE_NAME).o $(DEVICE_NAME).ko Module.symvers modules.order
	sudo modprobe -r $(DEVICE_NAME)
	sudo rm -f /lib/modules/$(shell uname -r)/kernel/drivers/misc/$(DEVICE_NAME).ko
	sudo depmod -a
	sudo rm -f /dev/$(DEVICE_NAME)

