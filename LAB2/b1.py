#!/usr/bin/env python3

import RPi.GPIO as GPIO
import sys
import time

DELAY=0.3
def initEnv():
    GPIO.setmode(GPIO.BOARD)

def initPin(pin):
    GPIO.setup(pin, GPIO.OUT)

def startVoice(pin):
    v = GPIO.PWM(pin, 50)
    v.start(50)
    return v

def mkVoice(v, f, d):
    v.ChangeFrequency(f)
    time.sleep(d)

def endVoice(v):
    v.stop()

def main():


    a=sys.argv[1]				
    PIN=int(a[1:len(a)-1])     
    
    b=sys.argv[2]
    Freq=[]
    c=b[1:len(b)-1]
    f=c.split(":")
    

    print("Env Init")
    initEnv()


    print("Get Freq")
    for i in range(len(f)):
    	Freq.append(float(f[i]))


    print("GPIO Init")
    initPin(PIN)
    v = startVoice(PIN)

    
    print("Playing")
    for fr in Freq:
        mkVoice(v, fr, DELAY)


    print("Ending")
    endVoice(v)
    GPIO.cleanup()

if __name__ == "__main__":
    main()
