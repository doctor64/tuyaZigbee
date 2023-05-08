# OTA update
Note: you cannot update with OTA device with tuya firmware, at least, currently

## Setup local OTA index for zigbee2mqtt
In data folder of zigbee2mqtt installation, edit file configuration.yaml and add
```
ota:
  zigbee_ota_override_index_location: index.json
```
Simply copy index.js and /images folder from OTA folder.  
If you want to add locally builded image see details on [zigbee2mqtt documentation](https://www.zigbee2mqtt.io/guide/usage/ota_updates.html#local-ota-index-and-firmware-files) how to create index file and section below.  
Note - currently need to patch add.js: add line `    25700: 'DocLab',` to `const manufacturerNameLookup = {`

## How to convert binary to OTA
Telink utility are crap, but works.  
Ater compilation, copy binary tl_zigbee_sdk\build\tlsr_tc32\TuyaLight\TuyaLignt.bin to Tools\OTA\zbGitVersionCode.bin  
run tl_ota_tool_1.0.exe  
Enter data as follow:
```
Input the Manufaurer Code(u16 hex value)):
6464
Input the OTA Image Type(u16 hex value)):
0301
Input the OTA File version(u32 hex value)):
10023001
```
for current version, see version_cfg.h  
Manufaurer is not a typo, it is output from Telink utility!  
Output file ota.ota ready



