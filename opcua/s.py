#!/usr/bin/env python3
import RPi.GPIO as GPIO
import signal
pins={'r':3,'g':5,'b':7}

def fuck(you):
    GPIO.setmode(GPIO.BOARD)
    for i in pins:GPIO.setup(pins[i], GPIO.OUT)
    pwmr = GPIO.PWM(pins['r'], 2000)
    pwmg = GPIO.PWM(pins['g'], 2000)
    pwmb = GPIO.PWM(pins['b'], 2000)
    pwmr.start(0)
    pwmg.start(0)
    pwmb.start(0)
    you=you%4
    if   you == 1:
        print('r')
        pwmr.ChangeDutyCycle(50)
        pwmg.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(0)
    elif you == 2:
        print('g')
        pwmr.ChangeDutyCycle(0)
        pwmg.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(0)
    elif you == 3:
        pwmr.ChangeDutyCycle(0)
        pwmg.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(50)
    else:
        print('close')
        pwmr.ChangeDutyCycle(0)
        pwmg.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(0)

def end_service():
    pwmg.stop()
    pwmb.stop()
    pwmr.stop()
    print('\n[INFO] Ctrl+C captured, shutdown service.')
    GPIO.cleanup()
