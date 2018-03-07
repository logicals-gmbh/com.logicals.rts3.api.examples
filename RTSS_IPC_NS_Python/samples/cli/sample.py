from RTS.IPCNS.Client import Client
from RTS.IPCNS.Exceptions import IPCNSRCError, IPCNSRCErrorFactory, IPCNSTimeoutError

import sys

def main():
    if len(sys.argv) != 3:
        print('Usage: ' + sys.argv[0] + ' <var-name> <var-value>')
        sys.exit(1)
    client = None
    try:
        client = Client('/tmp/build.rts3.linux-x64')
        client.connect()
        client.ping()
        client.writeValue(sys.argv[1], sys.argv[2])
        print('Variable value: ' + client.readValue(sys.argv[1]))
    except IPCNSRCError as error:
        print('Problem: \'%s\' (error 0x%08x).' % (str(error), error.errorCode))
    except OSError as error:
        print('OSError: ' + str(error))

if __name__ == "__main__":
    main()

