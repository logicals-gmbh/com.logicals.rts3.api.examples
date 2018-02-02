#include <RTSS_IPC_NS_API.h>

#include <cJSON.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void writeVariableValues(IPC_NS_Client* ipcNsClient, char const* json)
{
  char valBuf[64];
  char typeBuf[255];
  IPC_NS_Rc ipcRcVarRead;
  IPC_NS_Rc ipcRcVarType;
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

  fprintf(stdout, "{\n");
  cJSON_ArrayForEach(jsonVariable, jsonHandle)
  {
    if (!cJSON_IsString(jsonVariable))
    {
      continue;
    }

    ipcRcVarRead = rt_ipc_ns_read_var(ipcNsClient, valBuf, sizeof(valBuf), jsonVariable->valuestring);
    ipcRcVarType = rt_ipc_ns_get_typename(ipcNsClient, typeBuf, sizeof(typeBuf), jsonVariable->valuestring);
    if (RTSS_IPC_NS_SUCCEEDED(ipcRcVarRead) && RTSS_IPC_NS_SUCCEEDED(ipcRcVarType))
    {
      fprintf(stderr, "%s  \"%s\": {\n", isFirst ? "" : ",\n", jsonVariable->valuestring);
      fprintf(stderr, "    \"value\": \"%s\",\n", valBuf);
      fprintf(stderr, "    \"type\": \"%s\"\n", typeBuf);
      fprintf(stderr, "  }");
      isFirst = false;
    }
  }
  fprintf(stdout, "\n}\n");

  cJSON_Delete(jsonHandle);
}

/*
 * ./readVars '["COUNTER.COUNT", "COUNTER.UP"]'
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
