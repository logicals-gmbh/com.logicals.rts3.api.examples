if (NOT (RTS_TARGET_PLATFORM MATCHES "linux-arm11" OR
     RTS_TARGET_PLATFORM MATCHES "linux-x86" OR
     RTS_TARGET_PLATFORM MATCHES "nt-x86"
    )
  )
  message(FATAL_ERROR "Platform not supported.")
endif()

if (NOT RTS_SS_SDK_DIR)
  message(FATAL_ERROR "RTS_SS_SDK_DIR has not been set.")
endif ()

add_subdirectory("${RTS_SS_SDK_DIR}" "${CMAKE_BINARY_DIR}/rts3_sdk")
include(BuildDefaults)

find_library(RTSS_IPC_NS_API_LIB NAMES RTSS_IPC_NS_API PATHS ${SDK_LIB_DIR})

