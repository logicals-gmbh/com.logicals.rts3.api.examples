from .Exceptions import NotConnectedError, IPCNSRCErrorFactory

from os.path import join
from cffi import FFI


class Client(object):

    def __init__(self, sdkpath):
        self.ffi = FFI()
        self.ffi.cdef("""
                typedef unsigned int IPC_NS_Rc;
                typedef unsigned int uint8_t;
        """)
        self.ffi.cdef("""
                typedef struct _IPC_NS_Client IPC_NS_Client;

                IPC_NS_Client* rt_ipc_ns_open(void);
                IPC_NS_Rc rt_ipc_ns_ping(IPC_NS_Client* client);
                IPC_NS_Rc rt_ipc_ns_read_var(IPC_NS_Client* client, char* value, uint8_t valueLen, char const* name);
                IPC_NS_Rc rt_ipc_ns_write_var(IPC_NS_Client* client, char const* name, char const* value);
                void rt_ipc_ns_close(IPC_NS_Client* client);
        """)
        self.ipc_ns_api = self.ffi.dlopen(join(sdkpath, 'lib', 'libRTSS_IPC_NS_API.so'))
        self.handle = FFI.NULL

    def __del__(self):
        if self.isConnected():
            self.close()

    def connect(self):
        self.handle = self.ipc_ns_api.rt_ipc_ns_open()
        if not self.isConnected():
            raise NotConnectedError()

    def close(self):
        if not self.isConnected():
            raise NotConnectedError()
        self.ipc_ns_api.rt_ipc_ns_close(self.handle)
        self.handle = FFI.NULL

    def isConnected(self):
        return self.handle != FFI.NULL

    def ping(self):
        if not self.isConnected():
            raise NotConnectedError()

        rc = self.ipc_ns_api.rt_ipc_ns_ping(self.handle)
        IPCNSRCErrorFactory.handle(rc)

    def readValue(self, instancePath):
        if not self.isConnected():
            raise NotConnectedError()
        value = self.ffi.new("char[128]")
        rc = self.ipc_ns_api.rt_ipc_ns_read_var(
                self.handle, value, self.ffi.cast("char", len(value)), str.encode(instancePath))
        IPCNSRCErrorFactory.handle(rc, instancePath)
        return self.ffi.string(value).decode("utf-8")

    def writeValue(self, instancePath, value):
        if not self.isConnected():
            raise NotConnectedError()
        rc = self.ipc_ns_api.rt_ipc_ns_write_var(self.handle, str.encode(instancePath), str.encode(value))
        IPCNSRCErrorFactory.handle(rc, instancePath)

