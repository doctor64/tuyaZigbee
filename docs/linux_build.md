see https://github.com/Ai-Thinker-Open/Telink_825X_SDK

get linux toolchaim
wget http://shyboy.oss-cn-shenzhen.aliyuncs.com/readonly/tc32_gcc_v2.0.tar.bz2
install to /opt
```
sudo tar -xvjf tc32_gcc_v2.0.tar.bz2 -C /opt/
```
if needed add to PATH
```
export PATH=$PATH:/opt/tc32/bin
```
Configure build
```
cmake -DSDK_PREFIX=$(pwd)/../TelinkZSDK3685/tl_zigbee_sdk -DTOOLCHAIN_PREFIX=/opt/tc32 -B build -S .
```

Perform project build
```
cmake --build build --target IASsensor.zigbee
```
