#!/usr/bin/env python3
import RPi.GPIO as GPIO
import Adafruit_DHT as DHT
import time

shining= True

def initPin(pin):
    GPIO.setup(pin, GPIO.OUT)
    pwm = GPIO.PWM(pin, 1000)
    pwm.start(0)
    return pwm
#-----------------------MAIN------------------------#
def main():
    GPIO.setmode(GPIO.BOARD)
    BCM_PIN = 18
    pwmb=initPin(7)
    pwmfan = initPin(40)
    pwmb.ChangeDutyCycle(70)
    pwmfan.ChangeDutyCycle(100)
    duty=100
    while shining:
        h, t = DHT.read_retry(11, BCM_PIN)
        print('now t: ',t,'\nnow duty: ',duty)
        time.sleep(5)

if __name__ =='__main__':
    main()