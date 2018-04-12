
#!/usr/bin/env python3
"""
This is a program for simple led control on RasberryPi(R)

@author: FATESAIKOU
@argv[1:]: ALL Pins for using
"""

import RPi.GPIO as GPIO
import sys
import random
import time
import signal

shining = True


def end_handler(signal, frame):
    global shining
    print("end of shining")
    shining = False


def initEnv():
    GPIO.setmode(GPIO.BOARD)


def initPin(pin,value,freq):
    pwms = []
    for ledPin in range(len(pin)):
        GPIO.setup(pin[ledPin], GPIO.OUT)
        t_pwm = GPIO.PWM(pin[ledPin], freq[ledPin])
        t_pwm.start(value[ledPin])

        pwms.append(t_pwm)

    return pwms


def setPin(pwms, duties):
    for i in range(len(pwms)):
        pwms[i].ChangeDutyCycle(duties[i])


def endShining(pwms):
    for pwm in pwms:
        pwm.stop()


def main():
    pin=[]
    value=[]
    freq=[]
    for s in sys.argv[1:]:
        x = s.split(':')
        y = x[0].split('[')
        z=x[2].split(']')
        pin.append(int(y[1]))
        value.append(int(x[1]))
        freq.append(int(z[0]))

    initEnv()

    pwms = initPin(pin,value,freq)

    signal.signal(signal.SIGINT, end_handler)

    while shining:
        setPin(pwms, value)

    endShining(pwms)
    GPIO.cleanup()

if __name__ == '__main__':
    main()
