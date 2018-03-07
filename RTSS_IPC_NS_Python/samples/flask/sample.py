from RTS.IPCNS.Client import Client
from RTS.IPCNS.Exceptions import NotConnectedError, UnknownVariableError

from flask import Flask, make_response

app = Flask(__name__)
client = Client('/tmp/build.rts3.linux-x64')
client.connect()

@app.route('/read/<varname>')
def readValue(varname):
    try:
        return client.readValue(varname)
    except UnknownVariableError as error:
        return make_response(
                'Variable \'%s\' not found (error 0x%08x).' % (str(error), error.errorCode),
                404)
