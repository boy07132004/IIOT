#!/usr/bin/env python3
"""
This is a program for simple led control on RasberryPi(R)

@author: FATESAIKOU
@argv[1:]: ALL Pins for using
"""

import RPi.GPIO as GPIO
import sys
import time
import signal

shining = True

def end_handler(signal, frame): 
    global shining
    print("end of shining")
    shining = False

def initEnv():
    GPIO.setmode(GPIO.BOARD)

def initPin(pins):
    for ledPin in pins:
        GPIO.setup(ledPin, GPIO.OUT)

def setupPin(pins):
    for ledPin in pins:
        GPIO.output(ledPin, True)

def setoffPin(pins):
    for ledPin in pins:
        GPIO.output(ledPin, False)

def main():
    #PINS = [int(s) for s in sys.argv[1:]]
    PINS = []
    PINSS = []
    for s in sys.argv[1:]:
 
        x=s.split(':')
        y=x[0].split('[')
        z=x[1].split(']')
        PINS.append(int(y[1]))  
        if z[0] == 'true':
            PINSS.append(1)    
        elif z[0] == 'false':
            PINSS.append(0)
        
        
    initEnv()
    initPin(PINS)

    signal.signal(signal.SIGINT, end_handler)

    while shining:
        
        for i in range(len(PINS)):
            if PINSS[i] == "1":
                setupPin(PINS[i])    
            if PINSS[i] == "0":
                setoffPin(PINS[i])
            
    
    GPIO.cleanup()


if __name__ == '__main__':
    main()
