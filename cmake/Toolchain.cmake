# We will always be building Release in order to enable all needed optimizations
# Debug symbols will be still present in the ELF file
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR tc32)

if(CMAKE_HOST_WIN32)
  message(STATUS "Running on Windows host")
  set(TOOLCHAIN_PREFIX "${CMAKE_BINARY_DIR}/app/opt/tc32" CACHE PATH "toolchain prefix")
  set(CMAKE_C_COMPILER tc32-elf-gcc.exe)
  set(CMAKE_CXX_COMPILER tc32-elf-cpp.exe)
  set(CMAKE_ASM_COMPILER tc32-elf-gcc.exe)
  set(CMAKE_LD tc32-elf-ld.exe)
  set(CMAKE_OBJCOPY tc32-elf-objcopy.exe)
  set(CMAKE_OBJDUMP tc32-elf-objdump.exe)
  set(CMAKE_AR tc32-elf-ar.exe)
elseif(CMAKE_HOST_UNIX)
  message(STATUS "Running on Linux/MacOS host")
  set(TOOLCHAIN_PREFIX "${CMAKE_BINARY_DIR}/tc32" CACHE PATH "toolchain prefix")
  set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}/bin/tc32-elf-gcc)
  set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}/bin/tc32-elf-cpp)
  set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}/bin/tc32-elf-gcc)
  set(CMAKE_LD ${TOOLCHAIN_PREFIX}/bin/tc32-elf-ld)
  set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}/bin/tc32-elf-objcopy)
  set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}/bin/tc32-elf-objdump)
  set(CMAKE_AR ${TOOLCHAIN_PREFIX}/bin/tc32-elf-ar)
else()
  message(ERROR "Running on unknown host")
endif()

set(TELINK_PLATFORM "8258")

set(CMAKE_C_FLAGS "-O2 -ffunction-sections -fdata-sections -Wall -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -nostdlib" CACHE INTERNAL "c compiler flags")
set(CMAKE_ASM_FLAGS "-fomit-frame-pointer -fshort-enums -Wall -Wpacked -Wcast-align -fdata-sections -ffunction-sections -fno-use-cxa-atexit -fno-threadsafe-statics" CACHE INTERNAL "asm compiler flags")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -Wl,-uss_apsmeSwitchKeyReq -Os -fshort-enums -nostartfiles -fno-rtti -fno-exceptions -fno-use-cxa-atexit -fno-threadsafe-statics -Wl,--gc-sections " CACHE INTERNAL "executable linker flags")

MACRO(REMOVE_C_FLAG flag)
    string(REPLACE "${flag}" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
ENDMACRO()

# reduce size by 15% using -O2 instead of -O3
REMOVE_C_FLAG("-O3")

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(SDK_PREFIX "${CMAKE_BINARY_DIR}/tl_zigbee_sdk" CACHE PATH "sdk prefix")
set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_PREFIX}/bin)
set(TOOLCHAIN_INC_DIR ${TOOLCHAIN_PREFIX}/include)
set(TOOLCHAIN_LIB_DIR ${TOOLCHAIN_PREFIX}/lib)

function(add_bin_target TARGET TOOLS_PATH)
    if(EXECUTABLE_OUTPUT_PATH)
      set(FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}")
    else()
      set(FILENAME "${TARGET}")
    endif()
    add_custom_target("${TARGET}.bin" ALL
        DEPENDS ${TARGET}
        COMMAND ${TOOLCHAIN_BIN_DIR}/tc32-elf-objdump -x -D -l -S ${FILENAME} >${FILENAME}.lst
        COMMAND ${TOOLCHAIN_BIN_DIR}/tc32-elf-size -t ${FILENAME}
        COMMAND ${CMAKE_OBJCOPY} -Obinary ${FILENAME} ${FILENAME}.bin
        COMMAND ${Python3_EXECUTABLE} ${TOOLS_PATH}/tl_check_fw.py ${FILENAME}.bin
    )
endfunction()

function(add_ota_target TARGET TOOLS_PATH)
    if(EXECUTABLE_OUTPUT_PATH)
      set(FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}")
    else()
      set(FILENAME "${TARGET}")
    endif()
    add_custom_target("${TARGET}.zigbee" ALL
        DEPENDS ${TARGET}.bin
        COMMAND ${Python3_EXECUTABLE} ${TOOLS_PATH}/make_ota.py ${FILENAME}.bin
    )
endfunction()

function(add_tuya_ota_target TARGET TOOLS_PATH CODE TYPE VERSION)
    if(EXECUTABLE_OUTPUT_PATH)
      set(FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}")
    else()
      set(FILENAME "${TARGET}")
    endif()
    add_custom_target("${TARGET}.tuya.zigbee" ALL
        DEPENDS ${TARGET}.bin
        COMMAND ${Python3_EXECUTABLE} ${TOOLS_PATH}/make_ota.py -c ${CODE} -t ${TYPE} -v ${VERSION} ${FILENAME}.bin
    )
endfunction()

function(print_size_of_targets TARGET)
    if(EXECUTABLE_OUTPUT_PATH)
      set(FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}")
    else()
      set(FILENAME "${TARGET}")
    endif()
    add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_SIZE} ${FILENAME})
endfunction()
