# Windows Build

## Get Windows Toolchain and Zigbee SDK
> Ninja is required for building.
> ```powershell
> winget install Ninja-build.Ninja
> ```

```powershell
mkdir build; cd build; cmake -P ../cmake/TelinkSDK_Win.cmake
cd ..
```

## Configure Build
```powershell
cmake . -G "Ninja" -B build
```

### Custom Toolchain and SDK
```powershell
cmake . -G "Ninja" -B build -DTOOLCHAIN_PREFIX=~/opt/tc32 -DSDK_PREFIX=~/opt/tl_zigbee_sdk
```

## Perform Project Build
```powershell
cmake --build build --target IASsensor.zigbee
```
