TOUCH CONTROLLERS SUPPORTED
---------------------------

The TCM driver supports touch controllers based on the TouchComm communication
protocol.



DRIVER SOURCE
-------------

The source code of the driver and the reference hardware configuration can be
found in the /kernel directory inside the driver tarball. The following are the
files within this directory.

   drivers/input/touchscreen/synaptics_tcm/
      |
      |__synaptics_tcm_core.[ch]
      |     Source code of core driver module containing handling of TouchComm
      |     communication protocol
      |
      |__synaptics_tcm_i2c.c
      |     Source code of I2C module used for communicating with touch
      |     controllers using I2C
      |
      |__synaptics_tcm_spi.c
      |     Source code of SPI module used for communicating with touch
      |     controllers using SPI
      |
      |__synaptics_tcm_touch.c
      |     Source code of touch module used for reporting touch events to
      |     kernel input subsystem
      |
      |__synaptics_tcm_device.c
      |     Source code of TouchComm device module used for doing direct
      |     TouchComm access to touch controllers from user space via character
      |     device node
      |
      |__synaptics_tcm_reflash.c
      |     Source code of reflash module used for doing firmware and config
      |     update
      |
      |__synaptics_tcm_recovery.c
      |     Source code of recovery module used for doing microbootloader mode
      |     flash programming
      |
      |__synaptics_tcm_testing.[ch]
      |     Source code of testing module used for doing production tests
      |
      |__synaptics_tcm_diagnostics.c
      |     Source code of diagnostics module used for providing diagnostic
      |     reports to user space via sysfs interface
      |
      |__synaptics_tcm_zeroflash.c
      |     Source code of ZeroFlash module used for downloading firmware and
      |     config to touch controllers with no flash (microbootloader mode)
      |
      |__synaptics_tcm_romboot.c
      |     Source code of RomBoot module used for downloading firmware image
      |     to touch controllers with no flash (rombootloader mode)
      |
      |__Kconfig
      |     Kconfig for setting up build configuration of TCM driver
      |
      |__Makefile
            Makefile for building TCM driver

   arch/arm/boot/dts/qcom/
      |
      |__synaptics-tcm-i2c.dtsi
      |     Example device tree dtsi file for describing hardware setup of
      |     I2C-based touch controllers
      |
      |__synaptics-tcm-spi.dtsi
            Example device tree dtsi file for describing hardware setup of
            SPI-based touch controllers

   arch/arm64/configs/
      |
      |__msm8994_defconfig
            Example defconfig for 810 DragonBoard to include TCM driver in
            kernel build

   include/linux/input/
      |
      |__synaptics_tcm.h
            Header file shared between hardware configuration and driver

   firmware/
      |
      |__Makefile
            Example Makefile for including firmware images in kernel build for
            doing firmware update and host download

   Documentation/devicetree/bindings/input/touchscreen/synaptics_tcm/
      |
      |__synaptics_tcm_i2c.txt
      |     Descriptions of properties in device tree dtsi file for I2C-based
      |     touch controllers
      |
      |__synaptics_tcm_spi.txt
            Descriptions of properties in device tree dtsi file for SPI-based
            touch controllers


If can NOT be built in kernel, alternative example can be found in the below
directory inside the driver tarball, which copies firmware images to the
vendor/firmware/ directory during the system build.

   device/qcom/msm8994/
      |
      |__msm8994.mk
            Example modification for 810 DragonBoard to copy firmware images to
            the vendor/firmware/ directory


Also included in the driver tarball are the reference input device configuration
(.idc) and key layout (.kl) files for the TCM driver. These files are found in
the directories below inside the driver tarball and are to be placed in the
corresponding locations in the Android file system on the target platform.

   android/system/usr/
      |
      |__idc/synaptics_tcm.idc
      |     Example input device configuration file
      |
      |__keylayout/synaptics_tcm.kl
            Example key layout file



DRIVER PORTING
--------------

The procedures listed below describe the general process involved in porting the
driver to a target platform. Depending on the actual kernel BSP used for the
target platform, adjustments in the procedures and additional customization may
be needed.

1) Copy the synaptics_tcm folder in the kernel/driver/input/touchscreen
   directory of the driver tarball to the equivalent directory in the kernel
   source tree of the target platform.

2) Copy synaptics_tcm.h in the kernel/include/linux/input directory of the
   driver tarball to the equivalent directory in the kernel source tree of the
   target platform.

3) Add the line below inside Makefile in the drivers/input/touchscreen directory
   in the kernel source tree of the target platform.
      obj-$(CONFIG_TOUCHSCREEN_SYNAPTICS_TCM) += synaptics_tcm/

4) Add the line below inside Kconfig in the drivers/input/touchscreen directory
   in the kernel source tree of the target platform.
      source "drivers/input/touchscreen/synaptics_tcm/Kconfig"

5) Update the defconfig file in the kernel source tree of the target platform by
   referring to the example defconfig file in the driver tarball.
   * The following configuration options need to be enabled in the defconfig
     file.
        CONFIG_TOUCHSCREEN_SYNAPTICS_TCM
        CONFIG_TOUCHSCREEN_SYNAPTICS_TCM_CORE
   * One of the following configuration options needs to be enabled in the
     defconfig file based on the type of connection made with the touch
     controller on the target platform.
        CONFIG_TOUCHSCREEN_SYNAPTICS_TCM_I2C
        CONFIG_TOUCHSCREEN_SYNAPTICS_TCM_SPI
   * The other configuration options can be enabled as needed. The ones below
     are recommended.
        CONFIG_TOUCHSCREEN_SYNAPTICS_TCM_DEVICE
        For the discrete touch controller and the device with flash memory,
            CONFIG_TOUCHSCREEN_SYNAPTICS_TCM_REFLASH
        For the non-flash device,
            CONFIG_TOUCHSCREEN_SYNAPTICS_TCM_ZEROFLASH

6) Refer to the device tree dtsi files in the kernel/arch/arm/boot/dts/qcom
   directory of the driver tarball as examples and set up an equivalent dtsi
   file in the kernel source tree of the target platform to describe the
   hardware setup of the touch controller. This dtsi file needs to be included
   in the device tree build as appropriate.



DRIVER CONFIGURATION
--------------------

In addition to the dtsi file, the macros in the driver files below are used for
doing compile time configuration of the driver.

   synaptics_tcm_core.h
      |
      |__WAKEUP_GESTURE
      |     * If set to '1', the touch controller is put in low power wakeup
      |       gesture mode during suspend. When a valid gesture is detected, the
      |       driver is notified by the touch controller and proceeds to wake up
      |       the system.
      |     * If set to '0', the touch controller is put in sleep mode during
      |       suspend.
      |
      |__RD_CHUNK_SIZE
      |     Specifies the maximum number of bytes in a single read transaction
      |     allowed by the target platform. A value of 0 indicates no limit.
      |
      |__WR_CHUNK_SIZE
      |     Specifies the maximum number of bytes in a single write transaction
      |     allowed by the target platform. A value of 0 indicates no limit.
      |
      |__WATCHDOG_SW
            * If defined, the watchdog is enabled by default during driver
              initialization.
            * If not defined, the watchdog is disabled by default during driver
              initialization.

   synaptics_tcm_core.c
      |
      |__RESET_ON_RESUME
      |     * If defined, a software reset is issued to the touch controller
      |       when resuming from suspend.
      |     * If not defined, the touch controller is brought out of sleep mode
      |       and performs rezero when resuming from suspend.
      |
      |__PREDICTIVE_READING
      |     * If defined, predictive reading is used. When doing a read, the
      |       payload length is assumed to be at least the same as the previous
      |       read, and the read transaction covers that much payload. If the
      |       payload length of the current read turns out to be less than that
      |       of the previous read, the redundant data is discarded. If the
      |       payload length of the current read turns out to be greater than
      |       that of the previous read, a continued read is performed.
      |     * If not defined, two-stage reading is used. When doing a read, a
      |       four-byte read transaction is always performed first to find out
      |       the payload length of the current read. A continued read is then
      |       performed to read in the payload.
      |
      |__MIN_READ_LENGTH
      |     Specifies the minimum number of bytes to read in a read transaction
      |     when predictive reading is used. This is primarily used to ensure
      |     that enough payload is read in to cover at least a single set of
      |     touch data without requiring a further continued read in order to
      |     reduce the first touch response time.
      |
      |__FORCE_RUN_APPLICATION_FIRMWARE
      |     * If defined, after a spontaneous reset occurrence (i.e. a device
      |       reset not initiated by the driver) has been detected and the
      |       device remains in bootloader mode as a result, the driver
      |       automatically attempts to send the RUN_APPLICATION_FIRMWARE
      |       command to run the application firmware.
      |     * If not defined, after a spontaneous reset occurrence (i.e. a
      |       device reset not initiated by the driver) has been detected and
      |       the device remains in bootloader mode as a result, the driver
      |       does not automatically attempt to run the application firmware.
      |
      |__FALL_BACK_ON_POLLING
            * If defined, the driver falls back on using polling mode if
              interrupt handling by the driver fails to be enabled.
            * If not defined, the driver does not fall back on using polling
              mode if interrupt handling by the driver fails to be enabled.

   synaptics_tcm_touch.c
      |
      |__USE_DEFAULT_TOUCH_REPORT_CONFIG
            * If defined, the default touch report configuration that comes with
              the running firmware is used.
            * If not defined, the custom touch report configuration defined in
              the touch_set_report_config() function is used.

   synaptics_tcm_reflash.c
      |
      |__STARTUP_REFLASH
      |     * If defined, the driver attempts to perform reflash during system
      |       startup by using the firmware image defined by the FW_IMAGE_NAME
      |       macro. The driver uses the kernel's request_firmware() API to
      |       obtain the firmware image.
      |     * If not defined, the driver does not attempt to perform reflash
      |       during system startup.
      |
      |__FORCE_REFLASH
      |     * If set to true, when doing reflash, the driver always updates both
      |       the firmware and the config.
      |     * If set to false, when doing reflash, the driver compares the
      |       versions of the firmware runnning on the touch controller and in
      |       the firmware image. If the firmware in the firmware image is newer
      |       than the firmware running on the touch controller, both the
      |       firmware and the config are updated. If the firmware in the
      |       firmware image is the same as the firmware running on the touch
      |       controller, only the config is updated. If the firmware in the
      |       firmware image is older than the firmware running on the touch
      |       controller, no update is performed.
      |
      |__DISPLAY_REFLASH
            * If set to true, when upating the config during reflash, the
              display config is updated along with the application config.
            * If set to false, when updating the config during reflash, the
              display config is not updated.



SYSFS INTERFACE
---------------

The driver sets up a sysfs interface in the following location on the target
platform.
   /sys/bus/platform/devices/synaptics_tcm.0/synaptics_tcm/

The following are the directories and file nodes that make up this sysfs
interface.

   synaptics_tcm/
      |
      |__diagnostics/
      |     |__buttons
      |     |__cols
      |     |__data
      |     |__hybrid
      |     |__pid
      |     |__rows
      |     |__size
      |     |__type
      |
      |__dynamic_config/
      |     |__charger_connected
      |     |__disable_hsync
      |     |__disable_noise_mitigation
      |     |__enable_glove
      |     |__enable_thick_glove
      |     |__grip_suppression_enabled
      |     |__in_wakeup_gesture_mode
      |     |__inhibit_frequency_shift
      |     |__no_baseline_relaxation
      |     |__no_doze
      |     |__requested_frequency
      |     |__rezero_on_exit_deep_sleep
      |     |__stimulus_fingers
      |
      |__info
      |
      |__irq_en
      |
      |__recovery/
      |     |__ihex
      |     |__recovery
      |
      |__reflash/
      |     |__custom/
      |     |     |__lcm
      |     |     |__lockdown
      |     |     |__oem
      |     |__image
      |     |__reflash
      |
      |__reset
      |
      |__testing/
      |     |__data
      |     |__dynamic_range
      |     |__dynamic_range_doze
      |     |__dynamic_range_lpwg
      |     |__lockdown
      |     |__noise
      |     |__noise_doze
      |     |__noise_lpwg
      |     |__open_short_detector
      |     |__pt11
      |     |__pt12
      |     |__pt13
      |     |__reset_open
      |     |__size
      |     |__trx_trx_shorts
      |     |__trx_sensor_opens
      |     |__trx_ground_shorts
      |
      |__zeroflash/
            |__hdl

   * diagnostics/
        The file nodes contained in this directory are used by the SynaToolkit
        command-line tool for streaming out diagnostic reports. Refer to the
        README of SynaToolkit for more details.
   * dynamic_config/
        The file nodes contained in this directory are read/write file nodes
        representing individual dynamic config entries, which are firmware
        parameters that can be updated during runtime.
   * info
        This is a read-only file node that prints out the general information
        regarding the state of the touch controller including the firmware mode,
        part number, and packrat number.
   * irq_en
        This is a write-only file node that allows the enabling/disabling of
        interrupt handling by the driver. If polling mode is used instead, this
        file node is used to enable/disable the polling routine.
           0 = disable
           1 = enable
   * recovery/
        The file nodes contained in this directory are used for doing flash
        programming in microbootloader mode. More information can be found in
        the Flash Programming section below.
   * reflash/
        The file nodes contained in this directory are used for doing firmware
        update. More information can be found in the Firmware Update section
        below.
   * reflash/custom/
        The file nodes contained in this directory are used by the SynaToolkit
        command-line tool for accessing the custom data areas on the external
        flash. Refer to the README of SynaToolkit for more details.
   * reset
        This is a write-only file node that allows the sending of a reset to the
        touch controller.
           1 = software reset
           2 = hardware reset
   * testing/
        The file nodes contained in this directory are used for doing individual
        production tests. More information can be found in the Production Tests
        section below.
   * watchdog
        This is a write-only file node that allows the enabling/disabling of
        the watchdog in the driver.
           0 = disable
           1 = enable
   * zeroflash/
        The file nodes contained in this directory are used for doing host
        download in zeroflash mode. More information can be found in the
        Zeroflash section below.


FIRMWARE UPDATE
---------------

* Startup Firmware Update
     If the STARTUP_REFLASH macro in the reflash module of the driver is
     defined, the driver uses the kernel's request_firmware() API to obtain a
     default firmware image and do firmware update during system startup if
     necessary.

     There followings are two approaches to include firmware images in the kernel
     source tree of the target platform.

   1) BUILD_FIRMWARE_IN_KERNEL

       The default firmware image is expected to live in a file named
       reflash_firmware.img.ihex in the firmware/synaptics directory during the
       kernel build.

       To convert the .img firmware image provided by Synaptics to the ihex file
       format, use the command below.
          objcopy -I binary -O ihex <firmware_name>.img reflash_firmware.img.ihex

       To inlcude the firmware image in the kernel build, place the ihex file
       obtained using the command above in the firmware/synaptics directory in the
       kernel source tree of the target platform and make sure the line below is
       added inside Makefile in the firmware directory.
          fw-shipped-$(CONFIG_TOUCHSCREEN_SYNAPTICS_TCM_REFLASH) += synaptics/reflash_firmware.img

   2) COPY_FIRMWARE_TO_SYSTEM

       The default firmware image is expected to live in a file named
       reflash_firmware.img in the system/vendor/firmware/synaptics/ directory
       during the system build.

       To include the firmware image in the system build, place the .img firmware image
       file in the corresponding locations in the Android file system and modify the
       associated .mk Makefile.

       Example modification for 810 DragonBoard, put the .img firmware image into the
          device/qcom/msm8994/sensors/synaptics/ directory
       then, add the below lines in msm8994.mk.
          PRODUCT_COPY_FILES += \
          device/qcom/msm8994/sensors/synaptics/reflash_firmware.img:system/vendor/firmware/synaptics/reflash_firmware.img

* Runtime Firmware Update
     During runtime, firmware update can be triggered by writing to the reflash
     file node in the reflash directory of the sysfs interface described above.
     The reflash file node is write-only and takes a bit mask value with the
     following bit definitions.
        bit 0: reflash
        bit 1: force reflash
        bit 2: update application config
        bit 3: update display config
        bit 4: update boot config in next available slot
        bit 5: update boot config in lockdown slot

     The firmware image can come from the default firmware image built into the
     kernel binary and obtained using the kernel's request_firmware() API.
     Alternatively, the image file node in the reflash directory of the sysfs
     interface described above can be used to provide the driver with a new
     firmware image to use. The image file node is a write-only binary file node
     that can be fed with the firmware image by using tools such as 'dd'. For
     example, if the firmware image to use is /data/firmware.img on the target
     platform, then the command below can be used to feed the firmware image to
     the driver.
        dd if=/data/firmware.img of=/sys/bus/platform/devices/synaptics_tcm.0/synaptics_tcm/reflash/image

     After issuing the command above, the command below can then be used to
     trigger firmware update using the firmware image fed to the driver through
     the image file node.
        echo 1 > /sys/bus/platform/devices/synaptics_tcm.0/synaptics_tcm/reflash/reflash

     If the boot config is to be updated during the reflash process, the value
     to write to the reflash file node to trigger firmware update becomes 17
     (b'10001').
        echo 17 > /sys/bus/platform/devices/synaptics_tcm.0/synaptics_tcm/reflash/reflash


FLASH PROGRAMMING
-----------------

With touch controllers that implement a microbootloader in an internal mask ROM
and use an external flash for firmware and config storage (e.g. TDDI devices),
the entire flash can be erased and re-programmed through the microbootloader.
During runtime, flash programming can be triggered by making use of the file
nodes found in the recovery directory of the sysfs interface described above.
The ihex file node is a write-only binary file node that is used to provide the
driver with an ihex file to use for doing flash programming. For example, if the
ihex file to use is /data/flash.ihex on the target platform, then the dd command
below can be used to feed the ihex file to the driver.
   dd if=/data/flash.ihex of=/sys/bus/platform/devices/synaptics_tcm.0/synaptics_tcm/recovery/ihex

After issuing the command above, the flash programming process can be started by
writing to the recovery file node. The recovery file node is write-only and
takes one of two values depending on whether the touch controller is currently
running TouchComm firmware or already running in microbootloader mode.
   1 = touch controller currently running TouchComm firmware
   2 = touch controller already running in microbootloader mode

For example, if the touch controller is currently running TouchComm firmware,
the command below can be used to trigger flash programming using the ihex file
fed to the driver through the ihex file node.
   echo 1 > /sys/bus/platform/devices/synaptics_tcm.0/synaptics_tcm/recovery/recovery

Note that the ihex file used for doing flash programming is not the same as the
ihex file converted from a .img firmware image using objcopy for doing startup
firmware update. Instead, it should be provided separately by Synaptics.



ZEROFLASH
---------

In a ZeroFlash configuration where the touch controller implements a
microbootloader in an internal mask ROM (e.g. TDDI devices) but there is no
external flash for firmware and config storage, the driver carries out the host
download process to download both the firmware and the config to the touch
controller during runtime.

When doing host download, the driver uses the kernel's request_firmware() API to
obtain a firmware image from which the firmware and config data is retrieved.


There followings are two approaches to include firmware images in the kernel
source tree of the target platform.

 1)BUILD_FIRMWARE_IN_KERNEL

	 The firmware image is expected to live in a file named hdl_firmware.img.ihex in
	 the firmware/synaptics directory in the kernel source tree of the target
	 platform during the kernel build.

	 To convert the .img firmware image provided by Synaptics to the ihex file
	 format, use the command below.
	   objcopy -I binary -O ihex <firmware_name>.img hdl_firmware.img.ihex

	 To inlcude the firmware image in the kernel build, place the ihex file obtained
	 using the command above in the firmware/synaptics directory in the kernel source
	 tree of the target platform and make sure the line below is added inside
	 Makefile in the firmware directory.
	   fw-shipped-$(CONFIG_TOUCHSCREEN_SYNAPTICS_TCM_ZEROFLASH) += synaptics/hdl_firmware.img

 2)COPY_FIRMWARE_TO_SYSTEM

   The default firmware image is expected to live in a file
   named hdl_firmware.img in the system/vendor/firmware/synaptics/ directory
   during the system build.

   To include the firmware image in the system build, place the .img firmware image
   file in the corresponding locations in the Android file system and modify the
   associated .mk Makefile.

   Example modification for 810 DragonBoard, put the .img firmware image into the
       device/qcom/msm8994/sensors/synaptics/ directory
   then, add the below lines in msm8994.mk.
       PRODUCT_COPY_FILES += \
         device/qcom/msm8994/sensors/synaptics/hdl_firmware.img:system/vendor/firmware/synaptics/hdl_firmware.img


PRODUCTION TESTS
----------------

The file nodes found in the testing directory of the sysfs interface described
above are used to perform individual production tests. The following are the
test items currently supported and their associated file nodes.

   * Dynamic range test (dynamic_range)
   * Doze dynamic range test (dynamic_range_doze)
   * LPWG dynamic range test (dynamic_range_lpwg)
   * Noise test (noise)
   * Doze noise test (noise_doze)
   * LPWG noise test (noise_lpwg)
   * Open/short detector test (open_short_detector)
   * PT11 test (pt11)
   * PT12 test (pt12)
   * PT13 test (pt13)
   * Reset open test (reset_open)
   * Lockdown test (lockdown)
   * TRX-TRX shorts test (trx_trx_shorts)
   * TRX-sensor opens test (trx_sensor_opens)
   * TRX-ground shorts test (trx_ground_shorts)

The file nodes are read-only file nodes. When a file node is read, the driver
performs the corresponding test item and then returns the test result ("Passed"
or "Failed"). For example, the command below can be used to perform the dynamic
range test and get the test result.
   cat /sys/bus/platform/devices/synaptics_tcm.0/synaptics_tcm/testing/dynamic_range

The data and size file nodes do not represent test items. These file nodes are
instead used by the SynaToolkit command-line tool to retrive the detailed test
output. Refer to the README of SynaToolkit for more details.


