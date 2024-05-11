################################
# Common settings
cmake_minimum_required(VERSION 3.22)

#PROJECT(bootloader LANGUAGES C ASM)


SET(TARGET bootloader)

set(DEVICES_LIST "")
list(APPEND DEVICES_LIST "ZBWS01A")
#list(APPEND TARGETS_LIST ${TARGET}_1)
message(DEVICES_LIST=${DEVICES_LIST})


# ADD_DEFINITIONS
add_compile_definitions(
    -DMCU_CORE_${TELINK_PLATFORM}=1
    -DMCU_STARTUP_${TELINK_PLATFORM}=1
    -D__PROJECT_TL_BOOT_LOADER__=1
    -DBUILD_BOOTLOADER=1
)

################################
# Zigbee Library

SET(ZIGBEE_SRC
    ${SDK_PREFIX}/platform/boot/link_cfg.S
    ${SDK_PREFIX}/platform/boot/${TELINK_PLATFORM}/cstartup_${TELINK_PLATFORM}.S
    ${SDK_PREFIX}/platform/services/b85m/irq_handler.c
    ${SDK_PREFIX}/platform/tc32/div_mod.S
    ${SDK_PREFIX}/platform/chip_${TELINK_PLATFORM}/flash.c
    ${SDK_PREFIX}/platform/chip_${TELINK_PLATFORM}/adc.c
    ${SDK_PREFIX}/platform/chip_${TELINK_PLATFORM}/flash/flash_mid13325e.c

    ${SDK_PREFIX}/proj/common/list.c
    ${SDK_PREFIX}/proj/common/mempool.c
    ${SDK_PREFIX}/proj/common/string.c
    ${SDK_PREFIX}/proj/common/tlPrintf.c
    ${SDK_PREFIX}/proj/common/utility.c
    ${SDK_PREFIX}/proj/drivers/drv_adc.c
    ${SDK_PREFIX}/proj/drivers/drv_flash.c
    ${SDK_PREFIX}/proj/drivers/drv_calibration.c
    ${SDK_PREFIX}/proj/drivers/drv_gpio.c
    ${SDK_PREFIX}/proj/drivers/drv_hw.c
    ${SDK_PREFIX}/proj/drivers/drv_i2c.c
    ${SDK_PREFIX}/proj/drivers/drv_keyboard.c
    ${SDK_PREFIX}/proj/drivers/drv_nv.c
    ${SDK_PREFIX}/proj/drivers/drv_pm.c
    ${SDK_PREFIX}/proj/drivers/drv_putchar.c
    ${SDK_PREFIX}/proj/drivers/drv_pwm.c
    ${SDK_PREFIX}/proj/drivers/drv_spi.c
    ${SDK_PREFIX}/proj/drivers/drv_timer.c
    ${SDK_PREFIX}/proj/drivers/drv_uart.c
    ${SDK_PREFIX}/proj/os/ev.c
    ${SDK_PREFIX}/proj/os/ev_buffer.c
    ${SDK_PREFIX}/proj/os/ev_poll.c
    ${SDK_PREFIX}/proj/os/ev_queue.c
    ${SDK_PREFIX}/proj/os/ev_timer.c

    # ${SDK_PREFIX}/apps/common/main.c
    ${SDK_PREFIX}/zigbee/common/zb_version.c
)


################################
# Telink Zigbee device

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}

    # ${SDK_PREFIX}/zigbee/af
    # ${SDK_PREFIX}/zigbee/bdb/includes
    # ${SDK_PREFIX}/zigbee/ota
    # ${SDK_PREFIX}/zigbee/zcl
    # ${SDK_PREFIX}/zigbee/common/includes
    # ${SDK_PREFIX}/zigbee/zbapi
    ${SDK_PREFIX}/proj
    ${SDK_PREFIX}/proj/common
    ${SDK_PREFIX}/platform

    # Zigbee sources reference zcl_options.h
#    ${PROJECT_SOURCE_DIR}/src
#    ${PROJECT_SOURCE_DIR}/common/
    ${PROJECT_SOURCE_DIR}/bootloader
#    ${PROJECT_SOURCE_DIR}/src/custom_zcl
)

LINK_DIRECTORIES(
	${SDK_PREFIX}/zigbee/lib/tc32
	${SDK_PREFIX}/platform/lib
)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${SDK_PREFIX}/platform/boot/${TELINK_PLATFORM}/boot_${TELINK_PLATFORM}.link")

file( GLOB SOURCES1 *.c *.cpp *.h *.S common/*.c common/*.h )

SET (SOURCES  ${SOURCES1} ${ZIGBEE_SRC})

foreach(device_name IN LISTS DEVICES_LIST)
    ADD_EXECUTABLE(${TARGET}_${device_name} ${SOURCES})
    target_compile_definitions(${TARGET}_${device_name} PRIVATE BUILD_${device_name}=1)
    TARGET_LINK_LIBRARIES(${TARGET}_${device_name}
        -Wl,--start-group
	   drivers_${TELINK_PLATFORM}
#	timestamp
        -Wl,--end-group
    )
    ADD_BIN_TARGET(${TARGET}_${device_name} ${PROJECT_SOURCE_DIR}/tools)
endforeach()

# ADD_OTA_TARGET(${TARGET} ${PROJECT_SOURCE_DIR}/tools)
# ADD_TUYA_OTA_TARGET(${TARGET} ${PROJECT_SOURCE_DIR}/tools 0x1141 0xD3A3 0xFF)  


add_custom_target(${TARGET})
foreach(device_name IN LISTS DEVICES_LIST)
    message(device_name=${device_name})
    add_dependencies(bootloader ${TARGET}_${device_name})
    add_dependencies(bootloader ${TARGET}_${device_name}.bin)
endforeach()
