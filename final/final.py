#!/usr/bin/env python3
import RPi.GPIO as GPIO
import signal
import Adafruit_DHT as DHT
import time

dhting = True
shining= True

def end_handler(signal, frame):
    global shining
    print("end of shining")
    shining = False

def initLEDPin(pin):
    GPIO.setup(pin, GPIO.OUT)
    pwm = GPIO.PWM(pin, 1000)
    pwm.start(0)
    return pwm
#-----------------------MAIN------------------------#
def main():
    signal.signal(signal.SIGINT, end_handler)
    BCM_PIN = 12
    pwmr=initLEDPin(3)
    pwmg=initLEDPin(5)
    pwmb=initLEDPin(7)
    pwmg.stop()
    pwmb.stop()
    while shining:
        h, t = DHT.read_retry(11, BCM_PIN)
        if (t<30):
            duty=1
        elif (t>=30):
            duty=100
        pwmr.ChangeDutyCycle(0)
        print('now t: ',t,'\nnow duty: ',duty)
        time.sleep(5)
    # end
    pwmr.stop()
    pwmg.stop()
    pwmb.stop()
    GPIO.cleanup()
    
if __name__ == "__main__":
    main()