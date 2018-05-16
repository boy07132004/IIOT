#!/usr/bin/env python3

import RPi.GPIO as GPIO


# Client request handler
def main():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(3,GPIO.OUT)
    GPIO.setup(5,GPIO.OUT)
    GPIO.setup(7,GPIO.OUT)
    pwmr = GPIO.PWM(3,2000)
    pwmg = GPIO.PWM(5,2000)
    pwmb = GPIO.PWM(7,2000)
    
    pwmr.start(0)
    pwmg.start(0)
    pwmb.start(0)
    
    while True:
        pwmr.ChangeDutyCycle(50)
        pwmg.ChangeDutyCycle(50)
        pwmb.ChangeDutyCycle(50)

    pwmg.stop()
    pwmb.stop()
    pwmr.stop()

 

if __name__ == "__main__":
    main()