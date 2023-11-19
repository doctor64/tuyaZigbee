see https://github.com/Ai-Thinker-Open/Telink_825X_SDK

get linux toolchaim
wget http://shyboy.oss-cn-shenzhen.aliyuncs.com/readonly/tc32_gcc_v2.0.tar.bz2
install to /opt
sudo tar -xvjf tc32_gcc_v2.0.tar.bz2 -C /opt/
if needed add to PATH
export PATH=$PATH:/opt/tc32/bin

cmake -DSDK_PREFIX=$(pwd)/../TelinkZSDK3685/tl_zigbee_sdk -DTOOLCHAIN_PREFIX=/opt/tc32 -B build .
or if use clangd LSP
cmake -DSDK_PREFIX=$(pwd)/../TelinkZSDK3685/tl_zigbee_sdk -DTOOLCHAIN_PREFIX=/opt/tc32 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build .

cmake --build build --target z03mmc.zigbee