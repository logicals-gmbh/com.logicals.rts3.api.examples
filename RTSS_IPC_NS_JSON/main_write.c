#include <RTSS_IPC_NS_API.h>

#include <cJSON.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void writeVariableValues(IPC_NS_Client* ipcNsClient, char const* json)
{
  cJSON* jsonHandle;
  cJSON* jsonVariable;
  bool isFirst = true;

  jsonHandle = cJSON_Parse(json);

  if (jsonHandle == NULL)
  {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
    {
      fprintf(stderr, "Error before: %s\n", error_ptr);
    }
    return;
  }

  cJSON_ArrayForEach(jsonVariable, jsonHandle)
  {
    if (!cJSON_IsString(jsonVariable))
    {
      continue;
    }

    rt_ipc_ns_write_var(ipcNsClient, jsonVariable->string, jsonVariable->valuestring);
  }

  cJSON_Delete(jsonHandle);
}

/*
 * ./writeVars '{"COUNTER.COUNT": "1234", "COUNTER.UP": "TRUE"}'
 */
int main(int argc, char* argv[])
{
  IPC_NS_Client* ipcNsClient;
  int rc = 0;

  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <instance-path>\n", argv[0]);
    return 1;
  }

  ipcNsClient = rt_ipc_ns_open();
  if (!ipcNsClient)
  {
    fprintf(stderr, "Could not establish connection to run-time system.\n");
    return 1;
  }

  writeVariableValues(ipcNsClient, argv[1]);

  rt_ipc_ns_close(ipcNsClient);
  return rc;
}
