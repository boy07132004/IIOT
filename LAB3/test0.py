#!/usr/bin/env python3
import RPi.GPIO as GPIO
import sys
import signal

#========def========#
shining = True
print('shining')
def end_handler(signal, frame):
    global shining
    print("\nend of shining")
    shining = False

def initEnv():
    GPIO.setmode(GPIO.BOARD)

def initPin(pins):
    for ledPin in pins:
        GPIO.setup(ledPin, GPIO.OUT)

#========main========#
def main():
    signal.signal(signal.SIGINT, end_handler)

    PINS=[]
    PINS_VALUE=[]

    for s in sys.argv[1:]:
        a=s.split(":")
        PIN=a[0].split("[")
        PINS.append(int(PIN[1]))    # Which PINS
        c=a[1].split("]")
        PINS_VALUE.append(c[0])     # True or False

    nv()
    initPin(PINS)
    

    while shining:

        for i in range(len(PINS)):

            if PINS_VALUE[i]=="True" :
                GPIO.output(PINS[i], True)

            if PINS_VALUE[i]=="False":
                GPIO.output(PINS[i], False)
        
    GPIO.cleanup()


if __name__ == '__main__':
    main()
