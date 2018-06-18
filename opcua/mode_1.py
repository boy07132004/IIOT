#!/usr/bin/env python3
import RPi.GPIO as GPIO
import Adafruit_DHT as DHT
import time

def initPin(pin):
    GPIO.setup(pin, GPIO.OUT)
    pwm = GPIO.PWM(pin, 1000)
    pwm.start(0)
    return pwm
#-----------------------MAIN------------------------#
def main():
    GPIO.setmode(GPIO.BOARD)
    BCM_PIN = 18
    pwmr=initPin(3)
    pwmg=initPin(5)
    pwmfan = initPin(40)

    while 1:
        h, t = DHT.read_retry(11, BCM_PIN)
        if (t<30):
            pwmg.ChangeDutyCycle(70)
            pwmr.ChangeDutyCycle(0)
            pwmfan.ChangeDutyCycle(0)
        elif (t>=30):
            duty=5*t-150
            if duty>100:duty=100
            pwmg.ChangeDutyCycle(0)
            pwmr.ChangeDutyCycle(duty)
            pwmfan.ChangeDutyCycle(duty)

        print('now t: ',t,'\nnow duty: ',duty)
        time.sleep(5)
if __name__ == '__main__':
    main()
