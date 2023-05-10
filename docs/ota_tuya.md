# OTA update over Tuya firmware
Note: this method not work with all devices. It is works wit TS0041, but not work with TS0501B

# How to update Tuya device (TS0041) with zigbee2mqtt

## Prepare for OTA
1. Copy new converter for Tuya TS0041 to enable OTA operation.
   - Copy `converters/ts0041.js` to your z2m data folder
   - Add converter to z2m configuration.yaml. In section `external_converters:` add line `  - ts0041.js`
   - Repeat steps above to add converter for updated device, file `converters\Fixed_TS0041.js`
2. Copy OTA index file
   - Copy Releases/TuyaOTA/TS0041/index.js to your z2m data folder
   - Edit file configuration.yaml and add
```
ota:
  zigbee_ota_override_index_location: index.json
```
3. Restart z2m
4. Go to OTA tab in z2m web GUI, press `Check for new updates' button on Tuya device (keep device wake up by pressing button)
5. Wait for OTA finish
6. Remove Tuya device from z2m by using `Remove device` - `Force remove`

7. Restart z2m
8. Join updated device in usual manner. 
