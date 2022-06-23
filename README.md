
# Welcome to the Makeskyblue_wifi_Controller wiki!

***

![](https://github.com/lasitha-sparrow/Makeskyblue_wifi_Controller/blob/37e5ec2279ea984477610c4dc7a6a849a5fc0c81/PXL_20220528_161409441.thumb.jpg.4af1e9addfd34aa73dabe79133aa0260.jpg)

**The Main unit does not send any data unless it's first asked for data.  (With no WiFi comm, the TX/RX leads are dead silent.)  Checksum is the simple addition of all bytes in the packet AFTER the 0xAA header byte.**

![](https://github.com/lasitha-sparrow/Makeskyblue_wifi_Controller/blob/37e5ec2279ea984477610c4dc7a6a849a5fc0c81/IMG_6206.thumb.jpg.56e7d0516834359178ca17548227b3c5.jpg)
![](https://github.com/lasitha-sparrow/Makeskyblue_wifi_Controller/blob/e7d748f90985f33c523ca77f06d4d4d3a4b3c2f2/Untitled%20Sketch%202_bb.jpg)

it is observed the following "master output" data packets:

[0xAA] [0x55] [0x00 0x00 0x00] [checksum] -> "Read Status"

MSB responds ~300mS later with the following data string

[0xAA] [0xBB] [16: BatteryVoltage x.1] [16: BatteryAmps x.1] [16: SolarVolts x.1] [16: SolarWatts x1] [16: Temperature C x.1] [16: Cumulative KWH] [16: ???] [ModeFlags] [ErrorFlags] [0x00] [checksum]

## note: all "16" registers are 2-byte little-endian 16-bit values.  For example, battery voltage bytes of [0x53] [0x01] -> 0x0153 -> 339 decimal = 33.9v
* [ModeFlags]...only seen 1 bit ever get set:
* 7:
* 6:
* 5:
* 4:
* 3:
* 2: MPPT Mode Active
* 1:
* 0:
* [ErrorFlags]...only got 2 bits set, but maybe I haven't had too many errors yet...
* 7:
* 6:
* 5:
* 4:
* 3:
* 2:
* 1: Battery Overvoltage
* 0: Battery Undervoltage
* [0xAA] [0xCB] [register] [0x00 0x00 0x00 0x00 0x00] [checksum] -> "Read Config Register"
* [0xAA] [0xCA] [register] [16: data x1000] [0x00 0x00 0x00] [checksum] -> "Write Config Register"
* all data in these registers seems to be multiplied by 1,000 (decimal) for whatever reason
* register 0x00 -> special case function, see below
* register 0x01 -> MPPT Bulk Voltage * 1000 (i.e. 0x48 0x71 -> 0x7148 -> 29,000 -> 29.000v
* register 0x02 -> MPPT Float Voltage * 1000
* register 0x03 -> Out Timer * 1000 (why??!  5,000 decimal = 5 hours.  App won't let you do partial hours though.)
* register 0x04 -> MPPT Amps * 1000
* register 0x05 -> UVP Off Voltage * 1000
* register 0x06 -> UVP Recover Voltage * 1000
* register 0x07 -> COM Address * 1000 (pretty sure, app won't let this change)
* register 0x08 -> Battery Type * 1000 (0 = SLA, 1 = LiPo, 2 = LiLo, 3 = LiFe, 4 = LiTo--assuming, app won't accept anything other than SLA / LiPo)
* register 0x09 -> Battery Cells * 1000 (app doesn't seem to allow anything other than 2...at least for my 24v SLA battery test rig.)
* Regardless of whether function code is Read or Write, MSB responds ~150mS later with the following string:
* [0xAA] [0xDA] [register] [16: data x1000] [0x00 0x00 0x00] [checksum]

***
the "request data" packet is as follows: [0xAA] [0x55] [0x00 0x00 0x00] [checksum] -> "Read Status"

Without a proper "checksum", the MSB will not respond.  As the "checksum" is the simple sum of all the bytes AFTER the 0xAA "device ID" byte, the checksum for a read command will be 0x55.

So a full "read" packet should be: [0xAA] [0x55] [0x00] [0x00] [0x00] [0x55]

and the MSB should respond within 300mS

copied from Sid Genetry Solar
special thanks goes to Amjed Nizam
