see https://github.com/Ai-Thinker-Open/Telink_825X_SDK

get linux toolchaim
wget http://shyboy.oss-cn-shenzhen.aliyuncs.com/readonly/tc32_gcc_v2.0.tar.bz2 and install to /opt
```
sudo tar -xvjf tc32_gcc_v2.0.tar.bz2 -C /opt/
```
if needed add to PATH
```
export PATH=$PATH:/opt/tc32/bin
```
Get Zigbee SDK 
direct link to SDK http://wiki.telink-semi.cn/tools_and_sdk/Zigbee/Zigbee_SDK.zip
see documentation http://wiki.telink-semi.cn/doc/an/AN-19052900-E_Telink%20Zigbee%20SDK%20Developer%20Manual.pdf

Configure build
```
cmake -DSDK_PREFIX=$(pwd)/../TelinkZSDK3685/tl_zigbee_sdk -DTOOLCHAIN_PREFIX=/opt/tc32 -B build -S .
```

Perform project build
```
cmake --build build --target IASsensor.zigbee
```
