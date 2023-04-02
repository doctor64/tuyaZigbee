SDK installation
get IDE from http://wiki.telink-semi.cn/wiki/IDE-and-Tools/IDE-for-TLSR8-Chips/
get SDK from http://wiki.telink-semi.cn/wiki/chip-series/TLSR825x-Series/#software-development-kit
All-in-1 Zigbee SDK for TLSR9 and TLSR8

see http://wiki.telink-semi.cn/doc/an/AN_19052900-E_Telink%20Zigbee%20SDK%20Developer%20Manual.pdf

1. run TelinkSDKv1.3.1.exe and install
2. extract Zigbee_SDK somewhere
3. Start Telink IDE
4. Import Zigbee SDK project. File -> Import -> General -> Existing Projects into Workspace Select tlsr_tc32 in the tl_zigbee_sdk/build directory -> click “OK” 
4. Copy project settings - ti_zigbee_sdk - Build Configurations - Manage - New
5. Enter data: Name (TuyaLight), Description (Router-8258), select Copy settings from Existing configuration sampleLight_8258
6. Manage Configuration TuyaLight - press Set Active
7. Change path - project Properties - C/C++ Build - Settings (check TuyaLight is active configuration) -
Tool Settings
TC32 CC/Assembler - Paths - change "../../../apps/sampleLight" to path to project "../../../apps/TuyaLight"
TC32 Compiler - Directories - change "../../../apps/sampleLight" to path to project "../../../apps/TuyaLight"
8. add sources to Eclipse: in Project Explorer - apps RMB - New - Folder - Name - enter TuyaLight - Advanced - Link to alternative location - enter path to source
9. Change filter - project Properties - C/C++ General - Path and Symbols (check TuyaLight is active configuration) - Edit Filter - Add - apps/sampleLight
10. (recomended) Windows->Preferences->C++->Indexer Build configuration for the indexer: Use active build configuration