/* Copyright (C) logi.cals GmbH. All rights reserved. */

#include <stdio.h>
#include <stdint.h>

#include <RTSS_IPC_NS_API.h>

int main(void)
{
  IPC_NS_Client* ipcNsClient;
  IPC_NS_VarListIter* VarListIter;
  IPC_NS_Rc rc;
  uint32_t count = 0;

  ipcNsClient = rt_ipc_ns_open();
  if (!ipcNsClient)
  {
    fprintf(stderr, "Could not establish connection to run-time system.\n");
    return 1;
  }

  VarListIter = rt_ipc_ns_iter_create(ipcNsClient);
  if (!VarListIter)
  {
    fprintf(stderr, "Could not create iterator.\n");
    return 2;
  }

  printf("%30s | %30s | %10s | %6s\n", "INSTANCE PATH", "TYPE", "VALUE", "COUNT");
  rc = rt_ipc_ns_iter_first(VarListIter);
  while (RTSS_IPC_NS_SUCCEEDED(rc))
  {
    char valBuf[32];
    char const* instancePath;

    count += 1;

    instancePath = rt_ipc_ns_iter_data_get_name(VarListIter);
    rt_ipc_ns_read_var(ipcNsClient, valBuf, sizeof(valBuf), instancePath);
    printf("%30s | %40s | %10s | %6u\n",
        instancePath,
        rt_ipc_ns_iter_data_get_typename(VarListIter),
        valBuf,
        count);

    rc = rt_ipc_ns_iter_next(VarListIter);
  }
  rc = rt_ipc_ns_iter_close(VarListIter);

  if (RTSS_IPC_NS_FAILED(rc))
  {
    fprintf(stderr, "Could not close iterator.\n");
    return 3;
  }

#define RTSS_IPC_NS_SAMPLE_INSTANCE_PATH      "COUNTER.COUNT"
#define RTSS_IPC_NS_SAMPLE_VARIABLE_VALUE     "1234"
  rc = rt_ipc_ns_write_var(ipcNsClient, RTSS_IPC_NS_SAMPLE_INSTANCE_PATH, RTSS_IPC_NS_SAMPLE_VARIABLE_VALUE);
  if (RTSS_IPC_NS_FAILED(rc))
  {
    fprintf(stderr, "Could write value " RTSS_IPC_NS_SAMPLE_VARIABLE_VALUE \
        " to variable " RTSS_IPC_NS_SAMPLE_INSTANCE_PATH ".\n");
  }

  rt_ipc_ns_close(ipcNsClient);
  return 0;
}
