# Supported devices
Note, links to shops provided as reference only. I am not affiliated with these stores in any way. 

Also, be warned what Tuya names are completely unreliable. Tuya name, reported as Zigbee Model (ZCL attribute 0x0005 ModelIdentifier) is not a unique device name or model, but device class. For example, TS0501B is a light device single color, lots of completely diffrent devices can share same name. Tuya model name, reported as Zigbee Manufacturer (ZCL attribute 0x0004 ManufacturerName) too unique, and identical from hardware point of view devices can have different Id's.

## Light
| Name | Model | Photo | Zigbee Model | Zigbee Manufacturer | State | Links |
|------|-------|-------|--------------|---------------------|-------|-------|
| Led strip controller - one color | | | TS0501B | \_TZ3210\_9q49basr | Supported | [Aliexpress](https://www.aliexpress.com/item/1005004267226841.html) [Aliexpress2](https://www.aliexpress.com/item/1005005196855536.html) |

### Notes
From hardware point of view, all controllers (Single color/RGB/RGBW/RGBCW) the same, at least Single color (Tuya name TS0501B) and RGBW (Tuya name TS0504B), and differs only by firmware and external connector to LED strip. All controllers share same populated PCB and based on Tuya ZT3L module, which is based on Z2 chip - actually it is TeLink TLSR8258 (TLSR8258F1KAT32).

### Schematics
TS050X
Actually, part of it. I was lazy enough to trace 3.3 voltage converter :) 
![Controller schematics](docs/Schematic_TuYa_led_driver.png)

### How to join
Long press on button, connected LED will blink

### Features and improvements
- Short press on hardware button can be used as On/Off switch
- Working reporting of switching with hardware button
- Have a Power on behavior
- No more annoying bug with light goes off shortly afetr turn on (I hope)
- OTA updates

### Tasks
- [x] zigbee2mqtt converter
- [x] save state and power on control
- [x] reporting
- [x] OTA
- [ ] OTA update from Tuya firmware
- [ ] Reset by turning power on/off 5 times
- [ ] Direct control of LED PWM

## Switch
| Name | Model | Photo | Zigbee Model | Zigbee Manufacturer | State | Links |
|------|-------|-------|--------------|---------------------|-------|-------|
| Lonsonho 1 gang switch | ZBWS01A (on case) | | TS0041 | \_TZ3000\_s0i14ubi | Supported | [Aliexpress](https://www.aliexpress.com/item/4001095272467.html) white, 1-gang |
| Lonsonho 2 gang switch | ZBWS02A (on case) | | TS0042 | \_TZ3000\_v8jvcwsx | WIP | [Aliexpress](https://www.aliexpress.com/item/4001095272467.html) white, 2-gang |
| Lonsonho 3 gang switch | ZBWS03A (on case) | | TS0043 | \_TZ3000\_uyjmm0et | WIP | [Aliexpress](https://www.aliexpress.com/item/4001095272467.html) white, 3-gang |

### How to join
Long press on Net Key button

### Features and improvements
- Binding
- Short press on button send toggle command (switch binded light on/off)
- Long press on button send move command, so light brightness a changed. Direction is switched on next press
- Short click on Net Key button send report with battery status
- OTA updates
- OTA update from Tuya firmware

### Tasks
- [x] zigbee2mqtt converter
- [x] binding
- [x] reporting
- [x] OTA
- [x] OTA update from Tuya firmware
- [ ] Multiclick support

## Intruder Alarm System (IAS)
| Name | Model | Photo | Zigbee Model | Zigbee Manufacturer | State | Links |
|------|-------|-------|--------------|---------------------|-------|-------|
| IHOMECAM Door sensor with lux | ZG-102ZL | | TS0601 | \_TZE200\_pay2byax | WIP | [Aliexpress](https://www.aliexpress.com/item/1005004988882583.html) With Lux 1pc |

### How to join
Long press on button, LED will constantly light for 5 seconds then blink

### Features and improvements
- Support for IAS Enroll
- Binding to On/Off cluster, door sensor can directly turn on or off lights or similar devices
- Configurable commands send to binded devices
- Configurable network polling intervals accordingly to ZCL POLL_CONTROL cluster
- Configurable battery and light sensor polling interval
- Short press on button send report with battery status
- OTA updates

### Tasks
- [x] zigbee2mqtt converter
- [x] binding
- [x] reporting
- [x] OTA
- [x] POLL_CONTROL
- [x] converter support for POLL_CONTROL
- [ ] battery and sensor poll interval
- [x] converter support for sensor poll control
- [ ] OTA update from Tuya firmware
- [ ] Light sensor values unreliable
