ifneq ($(KERNELRELEASE),)
obj-m += kercli.o
else
#KERNELDIR ?=/lib/modules/$(shell uname -r)/build
KERNELDIR ?=/home/mike/kers/linux-4.2.3
PWD := $(shell pwd)

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

.PHONY: all 
endif
