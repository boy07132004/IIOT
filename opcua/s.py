#!/usr/bin/env python3
"""
@argv[1]: [server name]
"""

import RPi.GPIO as GPIO
import signal
import sys
 

#set PINS
pins={'r':2,'g':3,'b':4}
service_on = True

# Service ending handler
def end_service(signal, frame):
    global service_on
    print('\n[INFO] Ctrl+C captured, shutdown service.')
    pwmg.stop()
    pwmb.stop()
    pwmr.stop()
    GPIO.cleanup()
    service_on = False
    sys.exit(0)



# Client request handler
def main():

    GPIO.setmode(GPIO.BOARD)
    signal.signal(signal.SIGINT, end_service)
    
    for i in pins:GPIO.setup(pins[i],GPIO.OUT)
    pwmr = GPIO.PWM(pins['r'],2000)
    pwmg = GPIO.PWM(pins['g'],2000)
    pwmb = GPIO.PWM(pins['b'],2000)
    R,G,B=0,0,0
    pwmr.start(R)
    pwmg.start(G)
    pwmb.start(B)
        
    pwmr.ChangeDutyCycle(R)
    pwmg.ChangeDutyCycle(G)
    pwmb.ChangeDutyCycle(B)
           
    pwmg.stop()
    pwmb.stop()
    pwmr.stop()    



if __name__ == '__main__':
    main()