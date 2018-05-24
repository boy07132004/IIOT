#!/usr/bin/env python3
# PIN -> 7
import RPi.GPIO as GPIO

def main(LED):
    GPIO.cleanup()
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(7, GPIO.OUT)
    while LED:
        GPIO.output(7, True)
def OFF(LED)
    GPIO.cleanup()
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(7, GPIO.OUT)
    while LED:
        GPIO.output(7, False)
def clean():
    GPIO.cleanup()
