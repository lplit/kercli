ifneq ($(KERNELRELEASE),)
obj-m := kercli.o
else
  KERNELDIR ?=/lib/modules/$(shell uname -r)/build
  
all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

.PHONY: all 
endif
