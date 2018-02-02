cmake_minimum_required(VERSION 3.0)
project(SampleIPCNS_JSON C)

add_subdirectory(sdk)

### sample application
add_executable(RTS_IPC_NS_IterSample
  RTSS_IPC_NS_Sample.c
)

target_include_directories(RTS_IPC_NS_IterSample PRIVATE
  ${SDK_INC_DIR}/rtss/ipc_ns
)

target_link_libraries(RTS_IPC_NS_IterSample
  ${RTSS_IPC_NS_API_LIB}
)

### cJSON library
add_library(cjson STATIC
  lib/cJSON/cJSON.c
  lib/cJSON/cJSON_Utils.c
)

target_include_directories(cjson PUBLIC
  lib/cJSON
)

### readVars
add_executable(readVars
  main_read.c
)

target_include_directories(readVars PRIVATE
  ${SDK_INC_DIR}/rtss/ipc_ns
)

target_link_libraries(readVars
  ${RTSS_IPC_NS_API_LIB}
  cjson
)

### writeVars
add_executable(writeVars
  main_write.c
)

target_include_directories(writeVars PRIVATE
  ${SDK_INC_DIR}/rtss/ipc_ns
)

target_link_libraries(writeVars
  ${RTSS_IPC_NS_API_LIB}
  cjson
)
