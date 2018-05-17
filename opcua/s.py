import RPi.GPIO as GPIO
def init(a):
    GPIO.setmode(GPIO.BOARD)
    pins={'r':3,'g':5,'b':7}
    for i in pins:GPIO.setup(pins[i],GPIO.OUT)
    pwmr = GPIO.PWM(pins['r'],2000)
    pwmg = GPIO.PWM(pins['g'],2000)
    pwmb = GPIO.PWM(pins['b'],2000)
    pwmr.start(a)
    pwmg.start(50)
    pwmb.start(0)
def fuc(you):    
    init(0)
    you=you%4
    if you==1:
        print('r')
        pwmr.ChangeDutyCycle(90)
        pwmg.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(0)
    elif you==2:
        print('g')
        pwmr.ChangeDutyCycle(0)
        pwmg.ChangeDutyCycle(90)
        pwmb.ChangeDutyCycle(0)
    elif you==3:
        print('b')
        pwmr.ChangeDutyCycle(0)
        pwmg.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(90)
    else:
        print('close')
        pwmr.ChangeDutyCycle(0)
        pwmg.ChangeDutyCycle(0)
        pwmb.ChangeDutyCycle(0)
