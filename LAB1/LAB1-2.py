#!/usr/bin/env python3
import RPi.GPIO as GPIO
import sys
import signal

shining = True
#======================================#
def initPin(pins, freq, duties):
    pwms = []
    for i in range(len(pins)):
        GPIO.setup(pins[i], GPIO.OUT)
        t_pwm = GPIO.PWM(pins[i], freq[i])
        t_pwm.start(duties[i])
        pwms.append(t_pwm)
    return pwms

def endShining(pwms):
    for pwm in pwms:
        pwm.stop()

def end_handler(signal, frame):
    global shining
    print("end of shining")
    shining = False

def initEnv():
    GPIO.setmode(GPIO.BOARD)

def setPin(pwms, duties):
    for i in range(len(pwms)):
        pwms[i].ChangeDutyCycle(duties[i])

#=================================================#
def main():

    pin=[]
    p_d=[]
    p_f=[]

    for s in sys.argv[1:]:
        a=s.split(":")
        v=a[0].split("[")
        f=a[2].split("]")
        p_f.append(float(f[0]))
        pin.append(int(v[1]))
        p_d.append(int(a[1]))

    initEnv()
    pwms = initPin(pin,p_f,p_d)

    signal.signal(signal.SIGINT, end_handler)

    while shining:
        setPin(pwms,p_d)    



    print("end")
    endShining(pwms)
    GPIO.cleanup()


if __name__ == '__main__':
    main()

