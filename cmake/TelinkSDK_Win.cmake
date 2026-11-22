# Download and extract Telink Zigbee SDK
set(DOWNLOAD_URL_SDK "https://github.com/telink-semi/telink_zigbee_sdk/archive/refs/tags/V3.7.2.0.tar.gz")
set(SDK_EXPECTED_SHA256 "3c8dab1b3741409b9f1b01f83de0f16189c3c0256cf6a56f35c032a5e125c616")
set(DOWNLOAD_PATH_SDK "${CMAKE_CURRENT_BINARY_DIR}/telink_zigbee_sdk-3.7.2.0.tar.gz")
set(SDK_PREFIX_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/tl_zigbee_sdk")

# file(DOWNLOAD) can leave a truncated file behind on network errors, and an
# empty archive still makes "cmake -E tar" exit successfully. Retry the
# download and verify its checksum before anything is extracted.
function(telink_download url path sha256)
  set(attempt 0)
  while(attempt LESS 3)
    math(EXPR attempt "${attempt} + 1")
    message(STATUS "Downloading ${url} (attempt ${attempt}/3)")
    file(DOWNLOAD "${url}" "${path}" SHOW_PROGRESS STATUS dl_status TIMEOUT 600)
    list(GET dl_status 0 dl_code)
    list(GET dl_status 1 dl_msg)
    if(dl_code EQUAL 0 AND EXISTS "${path}")
      file(SHA256 "${path}" dl_sha256)
      if(dl_sha256 STREQUAL sha256)
        return()
      endif()
      message(WARNING "Checksum mismatch for ${path}: got ${dl_sha256}")
    else()
      message(WARNING "Download failed (${dl_code}): ${dl_msg}")
    endif()
    file(REMOVE "${path}")
  endwhile()
  message(FATAL_ERROR "Could not download ${url} with SHA256 ${sha256}")
endfunction()

if(NOT EXISTS ${DOWNLOAD_PATH_SDK})
  telink_download("${DOWNLOAD_URL_SDK}" "${DOWNLOAD_PATH_SDK}" "${SDK_EXPECTED_SHA256}")
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
set(DOWNLOAD_URL_TOOLCHAIN "http://wiki.telink-semi.cn/tools_and_sdk/Tools/IDE/Telink_IDE.zip")
set(TOOLCHAIN_EXPECTED_SHA256 "d47595aafae1b711a04dc83bab08467160ea2a3559ea976576c2f8a44f47bf1f")
set(DOWNLOAD_PATH_TOOLCHAIN "${CMAKE_CURRENT_BINARY_DIR}/Telink_IDE.zip")
set(TOOLCHAIN_INSTALLER "${CMAKE_CURRENT_BINARY_DIR}/IDE/TelinkSDKv1.3.1.exe")
set(TOOLCHAIN_INSTALL_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/app")
set(TOOLCHAIN_GCC "${TOOLCHAIN_INSTALL_LOCATION}/opt/tc32/bin/tc32-elf-gcc.exe")

set(DOWNLOAD_URL_INNOEXTRACT "https://github.com/dscharrer/innoextract/releases/download/1.9/innoextract-1.9-windows.zip")
set(INNOEXTRACT_EXPECTED_SHA256 "6989342c9b026a00a72a38f23b62a8e6a22cc5de69805cf47d68ac2fec993065")
set(DOWNLOAD_PATH_INNOEXTRACT "${CMAKE_CURRENT_BINARY_DIR}/innoextract-1.9-windows.zip")
set(INNOEXTRACT_BIN "${CMAKE_CURRENT_BINARY_DIR}/innoextract.exe")

if(NOT EXISTS ${TOOLCHAIN_INSTALLER})
  if(NOT EXISTS ${DOWNLOAD_PATH_TOOLCHAIN})
    telink_download("${DOWNLOAD_URL_TOOLCHAIN}" "${DOWNLOAD_PATH_TOOLCHAIN}" "${TOOLCHAIN_EXPECTED_SHA256}")
  endif()

  message(STATUS "Extracting toolchain installer to ${CMAKE_CURRENT_BINARY_DIR}/IDE")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${DOWNLOAD_PATH_TOOLCHAIN} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(NOT EXISTS ${TOOLCHAIN_INSTALLER})
  message(FATAL_ERROR "Telink IDE installer not found at ${TOOLCHAIN_INSTALLER}")
endif()

if(NOT EXISTS ${INNOEXTRACT_BIN})
  if(NOT EXISTS ${DOWNLOAD_PATH_INNOEXTRACT})
    telink_download("${DOWNLOAD_URL_INNOEXTRACT}" "${DOWNLOAD_PATH_INNOEXTRACT}" "${INNOEXTRACT_EXPECTED_SHA256}")
  endif()

  message(STATUS "Extracting innoextract to ${CMAKE_CURRENT_BINARY_DIR}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${DOWNLOAD_PATH_INNOEXTRACT} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(NOT EXISTS ${INNOEXTRACT_BIN})
  message(FATAL_ERROR "innoextract.exe not found at ${INNOEXTRACT_BIN}")
endif()

# The installer is an Inno Setup package; extract the tc32 toolchain from it.
if(NOT EXISTS ${TOOLCHAIN_GCC})
  message(STATUS "Extracting toolchain to ${TOOLCHAIN_INSTALL_LOCATION}")
  execute_process(COMMAND ${INNOEXTRACT_BIN} ${TOOLCHAIN_INSTALLER}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RESULT_VARIABLE innoextract_res)
  if(NOT innoextract_res EQUAL 0)
    message(FATAL_ERROR "innoextract failed with exit code ${innoextract_res}")
  endif()
endif()

if(NOT EXISTS ${TOOLCHAIN_GCC})
  message(FATAL_ERROR "tc32 toolchain not found at ${TOOLCHAIN_GCC}")
endif()
