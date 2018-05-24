#!/usr/bin/env python3
# PIN -> 7
import RPi.GPIO as GPIO
import signal

def initEnv():
    GPIO.setmode(GPIO.BOARD)

def initPin():
    GPIO.setup(7, GPIO.OUT)

def LEDDD():
        GPIO.output(7, True)     
def clean():
    GPIO.cleanup()