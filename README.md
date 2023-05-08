# tuyaZigbee
This project intended to replace firmwares in TuYa devices based on Telink chips
## Supported devices
* TS0501B, TS0504B (and possible others) LED controllers.
* TS0041 1 button battery switch
* More to come!

## Why?
Because TuYa firmware sucks, that's why :)
But, seriously, not counting common drawbacks of TuYa devices firmware like manufacturer dependent messages and overall strange approach to zigbee standart, TS050X devices have very annoying bug: sometimes, then device receive On command, it turn on light for few seconds and turned off. I was unable to find exact condition to trigger this bug, but it happens from time to time. TS0041 does not support binding, etc.
So, this project was born.
This project based on sample from TeLink zigbee SDK.
This is continuation of earlier project [TuyaLight](https://github.com/doctor64/tuyaLight)

## Improvements over original firmware
TS0501B
- Short press on hardware button can be used as On/Off switch
- Working reporting with binding or hardware button
- Have a Power on behavior
- No more annoying bug with light goes off shortly afetr turn on (I hope)
- OTA updates

TS0041
- Short click on button send toggle command (switch binded light on/off)
- Long press on button send move command, so light brightness a changed. Direction is switched on next press
- Short click on Net Key buttom send report with battery status

## Join
TS0501B
Long press on button, connected LED will blink

TS0041
Long press on Net Key button

# Compilation
See [docs/sdk_installation.md](docs/sdk_installation.md)

# Flashing
See [docs/flash.md](docs/flash.md)

# OTA update of Tuya firmware
See [docs/ota_tuya.md](docs/ota_tuya.md)

# OTA
See [docs/ota.md](docs/ota.md)

# TODO

## Known devices and status:
- TS0501B single Work in progress, mostly working
- TS0502B CCT Not started
- TS0503B RGB Not started
- TS0504B RGBW Work in progress
- TS0505B RGB/CCT Not started
- TS0041 1 button working

## Tasks
TS050X - Light
- [x] zigbee2mqtt converter
- [x] save state and power on control
- [x] reporting
- [x] OTA
- [ ] RGBW support
- [ ] Direct control of LED PWM using custom cluster
- [ ] Support of other controller types

TS0041 - Switch
- [x] zigbee2mqtt converter
- [x] binding
- [x] reporting
- [x] OTA
- [x] OTA update over Tuya firmware
- [ ] Multiclick support

# Technical details
TS050X
TuYa Zigbee LED controllers can be bought cheapely on Aliexpress. for example, [Here](https://www.aliexpress.com/item/1005005196855536.html). From hardware point of view, they all the same, at least TS0501B and TS0504B, and differs only by firmware and external connector to LED strip.
All controllers share same populated PCB and based on Tuya ZT3L module, which is based on Z2 chip - actually it is TeLink TLSR8258 (TLSR8258F1KAT32)

## Schematics
TS050X
Actually, part of it. I was lazy enough to trace 3.3 voltage converter :)
![Controller schematics](docs/Schematic_TuYa_led_driver.png)
