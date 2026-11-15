# Linux Build

## Get Linux Toolchain and Zigbee SDK
```bash
mkdir build && cd build && cmake -P ../cmake/TelinkSDK_Linux.cmake
cd ..
```

## Configure Build
```bash
cmake . -B build
```

### Custom Toolchain and SDK
```bash
cmake . -B build -DTOOLCHAIN_PREFIX=/opt/tc32 -DSDK_PREFIX=../telink_zigbee_sdk-3.7.2.0/tl_zigbee_sdk
```

## Perform Project Build
Current target list: `bootloader`, `IASsensor`, `light`, `switch`.
```bash
cmake --build build --target <target>
```
