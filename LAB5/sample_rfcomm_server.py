#!/usr/bin/env python3
"""
This program is a simple rfcomm server based on bluetooth connection,
please check your BlueTooth connection before you start your testing.

@author: FATESAIKOU
@argv[1]: server name
"""

import signal
import sys
from bluetooth import *


# Service ending handler
def end_service(signal, frame):
    global service_on
    print('[INFO] Ctrl+C captured, shutdown service.')
    service_on = False
    sys.exit(0)


# Client request handler
def handler(sock, info):
    global service_on
    print("[INFO] Accepted connection from", info)

    try:
        while service_on:
            data = sock.recv(1024)
            if len(data) == 0: break

            print("[RECV] %s" % data)
            sock.send('foo ' + data.decode('ascii'))
            print("[SEND] >> foo " + data.decode('ascii'))
    except IOError:
        pass
        

# Env init
UUID = '94f39d29-7d6d-437d-973b-fba39e49d4ee'
SERVER_NAME = sys.argv[1]
service_on = True


# Add terminator
signal.signal(signal.SIGINT, end_service)


# Create service socket
server_sock = BluetoothSocket(RFCOMM)
server_sock.bind( ('', PORT_ANY) )
server_sock.listen(1)
port = server_sock.getsockname()[1]


# Advertise your service
advertise_service(
    server_sock,
    SERVER_NAME,
    service_id      = UUID,
    service_classes = [ UUID, SERIAL_PORT_CLASS ],
    profiles        = [ SERIAL_PORT_PROFILE ]
)


# Start service
print('[INFO] Service listening at port %d' % port)

while service_on:
    c_sock, c_info = server_sock.accept()
    print('[INFO] Start of connection')
    handler(c_sock, c_info)
    print('[INFO] End of connection')
    c_sock.close()
    
server_sock.close()
print('[INFO] Service shutdown')
