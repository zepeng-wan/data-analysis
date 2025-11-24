# 头文件路径（我们把 synaptics_tcm.h 放在 src/include/linux/input/ 下）
ccflags-y += -I$(M)/include

# 先放行“隐式贯穿”告警（你后面有空再给源码补 fallthrough; 标注）
ccflags-y += -Wno-error=implicit-fallthrough

# === 核心模块：注意名字是 synaptics_tcm_core_module ===
obj-m += synaptics_tcm_core_module.o
synaptics_tcm_core_module-objs := \
  synaptics_tcm_core.o \
  synaptics_tcm_touch.o

# === 其余功能模块：各自独立 .ko（保持与仓库一致）===
obj-m += synaptics_tcm_device.o
obj-m += synaptics_tcm_diagnostics.o
obj-m += synaptics_tcm_reflash.o
obj-m += synaptics_tcm_recovery.o
obj-m += synaptics_tcm_testing.o
obj-m += synaptics_tcm_zeroflash.o

# === 总线驱动：可以同时生成 I2C 与 SPI 两个 .ko（上板按需加载）===
obj-m += synaptics_tcm_spi.o
obj-m += synaptics_tcm_i2c.o
# 如果你只会用 SPI，也可以把 I2C 这一行删掉，反之亦然
