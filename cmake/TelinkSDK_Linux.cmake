# Download and extract Telink Zigbee SDK
set(DOWNLOAD_URL_SDK "https://github.com/telink-semi/telink_zigbee_sdk/archive/refs/tags/V3.7.2.0.tar.gz")
set(SDK_EXPECTED_SHA256 "3c8dab1b3741409b9f1b01f83de0f16189c3c0256cf6a56f35c032a5e125c616")
set(DOWNLOAD_PATH_SDK "${CMAKE_CURRENT_BINARY_DIR}/telink_zigbee_sdk-3.7.2.0.tar.gz")
set(SDK_PREFIX_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/tl_zigbee_sdk")

if(NOT EXISTS ${SDK_PREFIX_LOCATION})
  if(NOT EXISTS ${DOWNLOAD_PATH_SDK})
    message(STATUS "Downloading SDK from ${DOWNLOAD_URL_SDK} to ${DOWNLOAD_PATH_SDK}")
    file(DOWNLOAD ${DOWNLOAD_URL_SDK} ${DOWNLOAD_PATH_SDK} SHOW_PROGRESS
      EXPECTED_HASH SHA256=${SDK_EXPECTED_SHA256})
  endif()

  message(STATUS "Extracting SDK to ${CMAKE_CURRENT_BINARY_DIR}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${DOWNLOAD_PATH_SDK} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  # GitHub archives extract into a <repo>-<tag>/ directory, while SDK_PREFIX
  # (cmake/Toolchain.cmake) expects tl_zigbee_sdk directly under build/.
  if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/telink_zigbee_sdk-3.7.2.0/tl_zigbee_sdk")
    message(STATUS "Moving SDK to ${SDK_PREFIX_LOCATION}")
    file(RENAME "${CMAKE_CURRENT_BINARY_DIR}/telink_zigbee_sdk-3.7.2.0/tl_zigbee_sdk" "${SDK_PREFIX_LOCATION}")
  endif()
endif()

if(NOT EXISTS ${SDK_PREFIX_LOCATION})
  message(FATAL_ERROR "Telink Zigbee SDK not found at ${SDK_PREFIX_LOCATION}")
endif()

# Download and extract Telink toolchain
set(DOWNLOAD_URL_TOOLCHAIN "http://shyboy.oss-cn-shenzhen.aliyuncs.com/readonly/tc32_gcc_v2.0.tar.bz2")
set(DOWNLOAD_PATH_TOOLCHAIN "${CMAKE_CURRENT_BINARY_DIR}/tc32_gcc_v2.0.tar.bz2")
set(TOOLCHAIN_PREFIX_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/tc32")

if(NOT EXISTS ${TOOLCHAIN_PREFIX_LOCATION})
  if(NOT EXISTS ${DOWNLOAD_PATH_TOOLCHAIN})
    message(STATUS "Downloading SDK from ${DOWNLOAD_URL_TOOLCHAIN} to ${DOWNLOAD_PATH_TOOLCHAIN}")
    file(DOWNLOAD ${DOWNLOAD_URL_TOOLCHAIN} ${DOWNLOAD_PATH_TOOLCHAIN} SHOW_PROGRESS)
  endif()

  message(STATUS "Extracting SDK to ${TOOLCHAIN_PREFIX_LOCATION}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${DOWNLOAD_PATH_TOOLCHAIN} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
