# Download and extract Telink Zigbee SDK
set(DOWNLOAD_URL_SDK "https://github.com/telink-semi/telink_zigbee_sdk/archive/refs/tags/V3.7.2.0.tar.gz")
set(SDK_EXPECTED_SHA256 "3c8dab1b3741409b9f1b01f83de0f16189c3c0256cf6a56f35c032a5e125c616")
set(DOWNLOAD_PATH_SDK "${CMAKE_CURRENT_BINARY_DIR}/telink_zigbee_sdk-3.7.2.0.tar.gz")
set(SDK_PREFIX_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/tl_zigbee_sdk")

if(NOT EXISTS ${DOWNLOAD_PATH_SDK})
  message(STATUS "Downloading SDK from ${DOWNLOAD_URL_SDK} to ${DOWNLOAD_PATH_SDK}")
  file(DOWNLOAD ${DOWNLOAD_URL_SDK} ${DOWNLOAD_PATH_SDK} SHOW_PROGRESS
    EXPECTED_HASH SHA256=${SDK_EXPECTED_SHA256})
endif()

if(NOT EXISTS ${SDK_PREFIX_LOCATION})
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
# set(DOWNLOAD_URL_TOOLCHAIN "https://www.dropbox.com/s/ths9rev0tvhhl96/Telink_IDE.zip?dl=1")
set(DOWNLOAD_URL_TOOLCHAIN "https://wiki.telink-semi.cn/tools_and_sdk/Tools/IDE/Telink_IDE.zip")
set(DOWNLOAD_PATH_TOOLCHAIN "${CMAKE_CURRENT_BINARY_DIR}/Telink_IDE.zip")
set(TOOLCHAIN_PREFIX_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/IDE")
set(TOOLCHAIN_INSTALL_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/app")
set(DOWNLOAD_URL_INNOEXTRACT "https://github.com/dscharrer/innoextract/releases/download/1.9/innoextract-1.9-windows.zip")
set(DOWNLOAD_PATH_INNOEXTRACT "${CMAKE_CURRENT_BINARY_DIR}/innoextract-1.9-windows.zip")
set(INNOEXTRACT_INSTALL_LOCATION "${CMAKE_CURRENT_BINARY_DIR}")

if(NOT EXISTS ${DOWNLOAD_PATH_TOOLCHAIN})
  message(STATUS "Downloading SDK from ${DOWNLOAD_URL_TOOLCHAIN} to ${DOWNLOAD_PATH_TOOLCHAIN}")
  file(DOWNLOAD ${DOWNLOAD_URL_TOOLCHAIN} ${DOWNLOAD_PATH_TOOLCHAIN} SHOW_PROGRESS)
endif()

if(NOT EXISTS ${DOWNLOAD_PATH_INNOEXTRACT})
  message(STATUS "Downloading innoextract from ${DOWNLOAD_URL_INNOEXTRACT} to ${DOWNLOAD_PATH_INNOEXTRACT}")
  file(DOWNLOAD ${DOWNLOAD_URL_INNOEXTRACT} ${DOWNLOAD_PATH_INNOEXTRACT} SHOW_PROGRESS)
endif()

if(NOT EXISTS ${TOOLCHAIN_PREFIX_LOCATION})
  message(STATUS "Extracting SDK to ${TOOLCHAIN_PREFIX_LOCATION}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${DOWNLOAD_PATH_TOOLCHAIN} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(NOT EXISTS ${INNOEXTRACT_INSTALL_LOCATION}/innoextract.exe)
  message(STATUS "Extracting innoextract to ${INNOEXTRACT_INSTALL_LOCATION}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${DOWNLOAD_PATH_INNOEXTRACT} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
  
# Toolchain needs to be extracted first (is innosetup installer)
if(NOT EXISTS ${TOOLCHAIN_INSTALL_LOCATION})
  message(STATUS "Extracting toolchain to ${TOOLCHAIN_INSTALL_LOCATION}")
  execute_process(COMMAND ${INNOEXTRACT_INSTALL_LOCATION}/innoextract.exe ${TOOLCHAIN_PREFIX_LOCATION}/TelinkSDKv1.3.1.exe
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
