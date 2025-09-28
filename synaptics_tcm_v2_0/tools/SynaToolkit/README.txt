Copyright (C) 2017-2018 Synaptics Incorporated. All rights reserved.

Use of the materials may require a license of intellectual property
from a third party or from Synaptics. Receipt or possession of this
file conveys no express or implied licenses to any intellectual
property rights belonging to Synaptics.

THIS PROGRAM IS PROVIDED "AS-IS,” AND SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS
AND IMPLIED WARRANTIES, INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF OR IN CONNECTION WITH THE USE OF THIS PROGRAM, HOWEVER CAUSED AND
BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE. IF A TRIBUNAL OF COMPETENT JURISDICTION DOES NOT PERMIT THE
DISCLAIMER OF DIRECT DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS’ TOTAL CUMULATIVE
LIABILITY TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.



TOOL FUNCTIONS
--------------

SynaToolkit is a command-line tool that interacts with the TCM driver to provide
the following tool functions. Details of each tool function can be found in the
sections below.
* Reflash
* Update Config
* Program Flash
* Custom Data
* Send Command
* Production Test
* Stream Report
* Write Packet
* Read Packet



INSTALLING SYNATOOLKIT
----------------------

SynaToolkit is installed by pushing the tool binary to the target device over
ADB and giving it appropriate executable permissions. The following are example
commands for installing SynaToolkit in the /data directory on the target device.
- adb root
- adb wait-for-devices
- adb push SynaToolkit /data
- adb shell chmod 777 /data/SynaToolkit

Alternatively, SynaToolkit can be placed in one of the locations on the target
device dedicated for storing command-line utilities (e.g. /system/bin).



RUNNING SYNATOOLKIT
-------------------

There are two ways to run SynaToolkit - through menu selection and through
single-line commands.
* Menu selection
  - Go into shell prompt on target device
  - Go to directory where SynaToolkit resides (if not included in PATH)
  - Run SynaToolkit to bring up menu selection
  - Select tool function
  - Enter parameters for selected tool function
* Single-line commands
  - Go into shell prompt on target device
  - Go to directory where SynaToolkit resides (if not included in PATH)
  - Run SynaToolkit and specify tool function and parameters in single command
       SynaToolkit [tool function] ...



REFLASH
-------

Use a firmware image (.img) file to update the firmware/config of the touch
controller.

Single-line command:
   SynaToolkit reflash [image file] [parameters...]

Parameters:
   -fr   Force reflash
   -bc   Include boot config update in next available slot
   -ld   Include boot config update in lockdown slot

Usage examples:
   - Perform reflash using PR1234567.img
        SynaToolkit reflash PR1234567.img
   - Perform force reflash using PR1234567.img
        SynaToolkit reflash PR1234567.img -fr
   - Perform reflash with boot config update using PR1234567.img
        SynaToolkit reflash PR1234567.img -bc
   - Perform force reflash with boot config lockdown using PR1234567.img
        SynaToolkit reflash PR1234567.img -fr -ld



UPDATE CONFIG
-------------

Use a firmware image (.img) file to update the config of the touch controller.

Single-line command:
   SynaToolkit update_config [image file] [parameters...]

Parameters:
   -ac   Update application config
   -dc   Update display config
   -bc   Update boot config in next available slot
   -ld   Update boot config in lockdown slot

Usage examples:
   - Update application and display config using PR1234567.img
        SynaToolkit update_config PR1234567.img -ac -dc
   - Update boot config using PR1234567.img
        SynaToolkit update_config PR1234567.img -bc



PROGRAM FLASH
-------------

Use an ihex file to erase and program the external flash of the touch
controller.

Single-line command:
   SynaToolkit program_flash [ihex file] [parameter]

Parameter:
   -ub   Touch controller already running in microbootloader mode

Usage examples:
   - Perform flash programming using PR1234567.ihex
        SynaToolkit program_flash PR1234567.ihex
   - Perform flash programming using PR1234567.ihex when touch controller is
     already running in microbootloader mode
        SynaToolkit program_flash PR1234567.ihex -ub



CUSTOM DATA
-----------

Read from and write to the custom data areas on the external flash of the touch
controller. When reading from a custom data area, the retrieved binary data is
stored in /data/custom_data_[custom area] on the target device. When writing to
a custom data area, the area is erased first before writing the payload data to
the area.

Single-line command:
   SynaToolkit custom_data [custom area] [-r] [read length] [-w] [payload]

Custom areas:
   lcm        LCM data area
   lockdown   Lockdown data area
   oem        OEM data area

Usage examples:
   - Read the entire data from the Lockdown data area
        SynaToolkit custom_data lockdown -r
   - Read 64 bytes from the LCM data area
        SynaToolkit custom_data lcm -r 64
   - Write $01 $02 $03 $04 $05 $06 $aa $bb to the OEM data area
        SynaToolkit custom_data oem -w 01 02 03 04 05 06 aa bb



SEND COMMAND
------------

Send a TouchComm/TouchBoot command and any accompanying payload to the touch
controller and receive the response data in stdout.

Single-line command:
   SynaToolkit send_command [command in hex] [payload in hex]

Usage examples:
   - Send command $20 and receive application info in stdout
        SynaToolkit send_command 20
   - Send command $24 to set field ID $07 of dynamic config to 1
        SynaToolkit send_command 24 07 01 00



PRODUCTION TEST
---------------

Run a production test and receive the test result in stdout.

Single-line command:
   SynaToolkit production_test [test item]

Test items:
   drt                   Dynamic range test
   drt_doze              Doze dynamic range test
   drt_lpwg              LPWG dynamic range test
   noise                 Noise test
   noise_doze            Doze noise test
   noise_lpwg            LPWG noise test
   open_short_detector   Open/short detector test
   pt11                  PT11 test
   pt12                  PT12 test
   pt13                  PT13 test
   reset_open            Reset open test
   lockdown              Lockdown test
   trx_trx_shorts        TRX-TRX shorts test
   trx_sensor_opens      TRX-sensor opens test
   trx_ground_shorts     TRX-ground shorts test

Usage example:
   - Run dynamic range test
        SynaToolkit production_test drt



STREAM REPORT
-------------

Continuously retrieve the selected report type from the touch controller until
the SynaToolkit process is terminated (e.g. using Ctrl+\). The binary data of
the retrieved reports is stored in /data/report_[report type in hex] on the
target device. Each entry of the report data is preceded by a 2-byte size field
of the entry in little-endian format. The data of the retrieved reports is also
formatted and sent to stdout when the SynaToolkit process is termintated.

Single-line command:
   SynaToolkit stream_report [report type in hex]

Usage example:
   - Stream out report type $12 (delta data)
        SynaToolkit stream_report 12



WRITE PACKET
------------

Send a raw packet to the touch controller. This tool function is expected to be
used in conjunction with Read Packet when doing remote debugging (with interrupt
handling and packet processing by the TCM driver disabled).

Single-line command:
   SynaToolkit write_packet [packet data in hex]

Usage example:
   - Send packet containing $23 $01 $00 $ff to touch controller
        SynaToolkit write_packet 23 01 00 ff



READ PACKET
-----------

Retrieve a raw packet from the touch controller. This tool function is expected
to be used in conjunction with Write Packet when doing remote debugging (with
interrupt handling and packet processing by the TCM driver disabled). The data
contained in the packet is sent to stdout.

Single-line command:
   SynaToolkit read_packet [length in bytes]

Usage example:
   - Retrieve 4-byte packet from touch controller
        SynaToolkit read_packet 4
