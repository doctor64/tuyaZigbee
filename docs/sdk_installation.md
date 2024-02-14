# SDK installation

## Download SDK and IDE (Toolchain)

+ get IDE from http://wiki.telink-semi.cn/wiki/IDE-and-Tools/IDE-for-TLSR8-Chips/
+ get SDK from http://wiki.telink-semi.cn/wiki/chip-series/TLSR825x-Series/#software-development-kit
All-in-1 Zigbee SDK for TLSR9 and TLSR8

> direct link to SDK http://wiki.telink-semi.cn/tools_and_sdk/Zigbee/Zigbee_SDK.zip
see http://wiki.telink-semi.cn/doc/an/AN-19052900-E_Telink%20Zigbee%20SDK%20Developer%20Manual.pdf

## Installation

1. run TelinkSDKv1.3.1.exe and install
2. extract Zigbee_SDK somewhere
3. Start Telink IDE
4. Import Zigbee SDK project. File -> Import -> General -> Existing Projects into Workspace Select tlsr_tc32 in the tl_zigbee_sdk/build directory -> click “OK” 
5. Copy project settings - ti_zigbee_sdk - Build Configurations - Manage - New
6. Enter data: Name (TuyaLight), Description (Router-8258), select Copy settings from Existing configuration sampleLight_8258
7. Change path - project Properties - C/C++ Build - Settings (check TuyaLight is active configuration) -
Tool Settings
TC32 CC/Assembler - Paths - change "../../../apps/sampleLight" to path to project "tuyaZigbee/light"
TC32 CC/Assembler - General Other GCC flags add -DBUILD_TS0501B=1
TC32 Compiler - Directories - change "../../../apps/sampleLight" to path to project "tuyaZigbee/light"
TC32 Compiler - Define Syms - add BUILD_TS0501B=1
8. add sources to Eclipse: in Project Explorer - apps RMB - New - Folder - Name - enter TuyaLight - Advanced - Link to alternative location - enter path to repo, ie tuyaZigbee
9. remove sample - project Properties - C/C++ General - Paths and Symbols - Source Locations - /tl_zigbee_sdk Edit Filter - Add apps/sampleLight
10. Manage Configuration TuyaLight - press Set Active
11. (recomended) Windows->Preferences->C++->Indexer Build configuration for the indexer: Use active build configuration
12. In tl_zigbee_sdk/apps/common/comm_cfg.h edit line #define BOOT_LOADER_MODE 0 to #define BOOT_LOADER_MODE 1

> In case of build problems try to delete tl_zigbee_sdk\build\tlsr_tc32\tuyaLight-0501 and 2-3 times do clean project in Eclipse

## Automatic Installation

```powershell
mkdir build; cd build; cmake -P ../cmake/TelinkSDK_Win.cmake
```
