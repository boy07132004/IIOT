#!/usr/bin/env python3
# PIN -> 7
import RPi.GPIO as GPIO
import signal

def initEnv():
    GPIO.setmode(GPIO.BOARD)

def initPin():
    GPIO.setup(7, GPIO.OUT)

def LEDDD():
    LEDD = True

    if LEDD == True:
        GPIO.output(7, True)
    elif LEDD == False:
        GPIO.output(7, False)
    return LEDD        
def clean():
    GPIO.cleanup()