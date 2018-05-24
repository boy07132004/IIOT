#!/usr/bin/env python3
# PIN -> 7
import RPi.GPIO as GPIO

def initEnv():
    GPIO.setmode(GPIO.BOARD)

def initPin():
    GPIO.setup(7, GPIO.OUT)

def LEDDD():
    while 1:
        GPIO.output(7, True)     
    print("123")
def clean():
    GPIO.cleanup()
