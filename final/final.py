#!/usr/bin/env python3
import RPi.GPIO as GPIO
import signal
import Adafruit_DHT as DHT
import time

shining= True

def end_handler(signal, frame):
    global shining
    print("end of shining")
    shining = False

def initPin(pin):
    GPIO.setup(pin, GPIO.OUT)
    pwm = GPIO.PWM(pin, 1000)
    pwm.start(0)
    return pwm
#-----------------------MAIN------------------------#
def mode_1():
    GPIO.setmode(GPIO.BOARD)
    signal.signal(signal.SIGINT, end_handler)
    BCM_PIN = 18
    pwmr=initPin(3)
    pwmg=initPin(5)
    pwmfan = initPin(40)

    while shining:
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
    # end
    pwmr.stop()
    pwmg.stop()
    pwmfan.stop()
    GPIO.cleanup()

def mode_2:
    GPIO.setmode(GPIO.BOARD)
    signal.signal(signal.SIGINT, end_handler)
    BCM_PIN = 18
    pwmb=initPin(7)
    pwmfan = initPin(40)
    pwmb.ChangeDutyCycle(70)
    pwmfan.ChangeDutyCycle(100)
    while shining:
        h, t = DHT.read_retry(11, BCM_PIN)
        print('now t: ',t,'\nnow duty: 100')
        time.sleep(5)
    # end
    pwmb.stop()
    pwmfan.stop()
    GPIO.cleanup()
if __name__ == "__main__":
    mode_2()