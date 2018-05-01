#!/usr/bin/env python3
"""
This program is a simple rfcomm server based on bluetooth connection,
please check your BlueTooth connection before you start your testing.

@author: FATESAIKOU
@argv[1]: server name
3:R 5:G 7:B
"""
import RPi.GPIO as GPIO
import signal
import sys
from bluetooth import *

pins={'r':3,'g':5,'b':7}




# Client request handler
def handler(sock, info):
    global service_on
    print("[INFO] Accepted connection from", info)
    GPIO.setmode(GPIO.BOARD)
    for i in pins :GPIO.setup(pins[i],GPIO.OUT)
        
    pwmr = GPIO.PWM(pins['r'],2000)
    pwmg = GPIO.PWM(pins['g'],2000)
    pwmb = GPIO.PWM(pins['b'],2000)
    pwmr.start(0)
    pwmg.start(0)
    pwmb.start(0)
    
    try:
        while service_on:
            data = sock.recv(1024)
            if len(data) == 0: break

            print("[RECV] %s" % data)
            sock.send('foo ' + data.decode('ascii'))
            rec=data.decode('ascii')
            rec1=rec.split(' ')
            print(data.decode('ascii'))
            
            if rec1[0] == 'get':
                print('get')
            elif rec1[0] == 'set':
                a=rec1[1].split(':')
                R=int(a[0])
                G=int(a[1])
                B=int(a[2])
                pwmr.ChangeDutyCycle(R)
                pwmg.ChangeDutyCycle(G)
                pwmb.ChangeDutyCycle(B)
            else:print('nope')
            print("[SEND] >> done " + data.decode('ascii'))
    except IOError:
        pass
        

# Service ending handler
def end_service(signal, frame):
    pwmg.stop()
    pwmb.stop()
    pwmr.stop()
    global service_on
    print('[INFO] Ctrl+C captured, shutdown service.')
    service_on = False
    GPIO.cleanup()
    sys.exit(0)

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
