#!/usr/bin/env python3
"""
@argv[1]: server name
PIN >> 3:R 5:G 7:B
"""
import RPi.GPIO as GPIO
import signal
import sys
from bluetooth import *

#set PINS
pins={'r':3,'g':5,'b':7}

# Service ending handler
def end_service(signal, frame):
    global service_on
    print('[INFO] Ctrl+C captured, shutdown service.')
    GPIO.cleanup()
    service_on = False
    sys.exit(0)



# Client request handler
def handler(sock, info):
    global service_on
    print("[INFO] Accepted connection from", info)
    GPIO.setmode(GPIO.BOARD)
    for i in pins:GPIO.setup(pins[i],GPIO.OUT)
    pwmr = GPIO.PWM(pins['r'],2000)
    pwmg = GPIO.PWM(pins['g'],2000)
    pwmb = GPIO.PWM(pins['b'],2000)
    R,G,B=0,0,0
    pwmr.start(R)
    pwmg.start(G)
    pwmb.start(B)
    
    try:
        while service_on:
            data = sock.recv(1024)
            if len(data) == 0: break

            print("[RECV] %s" % data)
            rec=data.decode('ascii')
            rec1=rec.split(' ')
                         
            if rec1[0:3] == ['get','led','values']:
                try:
                    string=str(R)+':'+str(G)+':'+str(B)
                    sock.send('NOW R:G:B >> '+string)
                    print(string)
                except:
                    sock.send('ERROR GET')
                    print('ERROR GET')
                    continue

            elif rec1[0:2] == ['set','led']:
                try:
                    a=rec1[2].split(':')
                    R=int(a[0])
                    G=int(a[1])
                    B=int(a[2])
                    pwmr.ChangeDutyCycle(R)
                    pwmg.ChangeDutyCycle(G)
                    pwmb.ChangeDutyCycle(B)
                    sock.send('DONE SET')
                except:
                    sock.send('ERROR SET')
                    print('ERROR SET')
                    continue
            else:
                sock.send('ERROR INPUT')
                print('ERROR INPUT')
                continue
            print("[SEND] >> done " + rec)
        pwmg.stop()
        pwmb.stop()
        pwmr.stop()
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
