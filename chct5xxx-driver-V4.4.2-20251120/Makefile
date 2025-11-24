KERNEL_DIR := /home/develop/source/linux-kernel/nxp/linux-imx-rel_imx_4.1.15_2.1.0_ga
CURRENT_PATH := $(shell pwd)
obj-m = stc.o

stc-objs += basic_util.o
stc-objs += spi_communication.o
#stc-objs += i2c_communication.o
stc-objs += semi_touch_function.o
stc-objs += semi_touch_upgrade.o
stc-objs += semi_touch_apk.o
stc-objs += semi_touch_device.o
stc-objs += semi_touch_custom.o
stc-objs += semi_touch_test_5xxx.o
stc-objs += semi_touch_thp_dev.o
stc-objs += qual_core.o


build: build_module

build_module:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_PATH) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_PATH) clean