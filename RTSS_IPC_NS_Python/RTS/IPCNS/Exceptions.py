

class IPCNSError(Exception):

    def __init__(self, msg=''):
        super().__init__(msg)


class IPCNSRCError(IPCNSError):

    RTSS_IPC_NS_RC_OK = 0

    def __init__(self, errorCode, msg=''):
        self.errorCode = errorCode
        super().__init__(msg)


class NotConnectedError(IPCNSError):

    def __init__(self, msg=''):
        super().__init__(msg)


class IPCNSOSError(IPCNSRCError):

    RTSS_IPC_NS_RC_OS_ERR = 0x80000001

    def __init__(self, msg):
        super().__init__(IPCNSOSError.RTSS_IPC_NS_RC_OS_ERR, msg)


class IPCNSInvParamError(IPCNSRCError):

    RTSS_IPC_NS_RC_INV_PARAM = 0x80000002

    def __init__(self, msg):
        super().__init__(IPCNSInvParamError.RTSS_IPC_NS_RC_INV_PARAM, msg)


class IPCNSNotImplError(IPCNSRCError):

    RTSS_IPC_NS_RC_NOT_IMPL = 0x80000003

    def __init__(self, msg):
        super().__init__(IPCNSInvParamError.RTSS_IPC_NS_RC_NOT_IMPL, msg)


class IPCNSRTSError(IPCNSRCError):

    RTSS_IPC_NS_RC_RTS_ERR = 0x80000004

    def __init__(self, msg):
        super().__init__(IPCNSRTSError.RTSS_IPC_NS_RC_RTS_ERR, msg)


class IPCNSTimeoutError(IPCNSRCError):

    RTSS_IPC_NS_RC_TIMEOUT_ERR = 0x80000005

    def __init__(self, msg):
        super().__init__(IPCNSTimeoutError.RTSS_IPC_NS_RC_TIMEOUT_ERR, msg)


class IPCNSRCErrorFactory(object):

    ERRORS = {
            IPCNSOSError.RTSS_IPC_NS_RC_OS_ERR: IPCNSOSError,
            IPCNSInvParamError.RTSS_IPC_NS_RC_INV_PARAM: IPCNSInvParamError,
            IPCNSNotImplError.RTSS_IPC_NS_RC_NOT_IMPL: IPCNSNotImplError,
            IPCNSRTSError.RTSS_IPC_NS_RC_RTS_ERR: IPCNSRTSError,
            IPCNSTimeoutError.RTSS_IPC_NS_RC_TIMEOUT_ERR: IPCNSTimeoutError,
    }

    @staticmethod
    def handle(rc, msg=''):
        if rc == IPCNSRCError.RTSS_IPC_NS_RC_OK:
            return
        if rc in IPCNSRCErrorFactory.ERRORS:
            raise IPCNSRCErrorFactory.ERRORS[rc](msg)
        raise IPCNSRCError(rc, 'Unknown error.')
