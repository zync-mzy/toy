obj-m := mod1.o
KDIR := /lib/modules/$(shell uname -r)/build
all:
	#$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* .c*.t* Module* module* .*
