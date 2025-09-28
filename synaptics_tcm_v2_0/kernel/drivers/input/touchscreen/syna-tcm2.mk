# ################################################################################
# # syna-tcm2
# ################################################################################

# SYNA_TCM2_VERSION = 1.0
# SYNA_TCM2_SITE = /home/work/sdk/rk_linux_sdk/buildroot/package/syna-tcm2/src
# SYNA_TCM2_SITE_METHOD = local

# SYNA_TCM2_MAKE_OPTS = \
#     KERNELDIR="$(LINUX_DIR)" \
#     KERNEL_BUILD_DIR="$(LINUX_BUILD_DIR)" \
#     ARCH=$(KERNEL_ARCH) \
#     CROSS_COMPILE="$(TARGET_CROSS)"

# define SYNA_TCM2_BUILD_CMDS
# 	$(MAKE) -C $(LINUX_DIR) \
# 		ARCH=$(KERNEL_ARCH) CROSS_COMPILE="$(TARGET_CROSS)" \
# 		M="$(@D)" modules
# endef


# define SYNA_TCM2_INSTALL_TARGET_CMDS
# 	mkdir -p $(TARGET_DIR)/lib/modules/$(LINUX_VERSION_PROBED)/extra
# 	find $(@D) -name '*.ko' -exec cp {} \
# 	  $(TARGET_DIR)/lib/modules/$(LINUX_VERSION_PROBED)/extra/ \;
# endef

# $(eval $(generic-package))

################################################################################
# syna-tcm2 (in-tree package)
################################################################################

SYNA_TCM2_VERSION = 1.0
# 关键：指向 Buildroot 顶层下的 package/syna-tcm2/src
SYNA_TCM2_SITE = $(TOPDIR)/package/syna-tcm2/src
SYNA_TCM2_SITE_METHOD = local

# 用内核 Kbuild 编外部模块：在内核目录 -C，指回本包目录 M="$(@D)"
define SYNA_TCM2_BUILD_CMDS
	$(MAKE) -C $(LINUX_DIR) \
		$(if $(LINUX_BUILD_DIR),O=$(LINUX_BUILD_DIR)) \
		ARCH=$(KERNEL_ARCH) CROSS_COMPILE="$(TARGET_CROSS)" \
		M="$(@D)" modules
endef

# 安装 .ko 到镜像（两种二选一）

# A) 简单拷贝到 /extra（保持你现在的做法）
define SYNA_TCM2_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/$(LINUX_VERSION_PROBED)/extra
	find $(@D) -name '*.ko' -exec cp {} \
	  $(TARGET_DIR)/lib/modules/$(LINUX_VERSION_PROBED)/extra/ \;
endef

# B) 或者使用模块安装（会放到标准目录并写入 modules.dep）
# define SYNA_TCM2_INSTALL_TARGET_CMDS
# 	$(MAKE) -C $(LINUX_DIR) \
# 		$(if $(LINUX_BUILD_DIR),O=$(LINUX_BUILD_DIR)) \
# 		ARCH=$(KERNEL_ARCH) CROSS_COMPILE="$(TARGET_CROSS)" \
# 		M="$(@D)" INSTALL_MOD_PATH="$(TARGET_DIR)" \
# 		modules_install
# endef

$(eval $(generic-package))


