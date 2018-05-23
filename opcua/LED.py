#!/usr/bin/env python3
# PIN -> 7
import RPi.GPIO as GPIO
import signal

#========def========#
shining = True

def end_handler():
    global shining
    print("end of shining")
    shining = False

def initEnv():
    GPIO.setmode(GPIO.BOARD)

def initPin():
    GPIO.setup(7, GPIO.OUT)

def LEDDD():
    shining = True
    LED=True
    while shining:
        if LED == True:
            GPIO.output(7, True)
        elif LED == False:
            GPIO.output(7, False)
            
def clean():
    GPIO.cleanup()