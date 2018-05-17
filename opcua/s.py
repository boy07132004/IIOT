import RPi.GPIO as GPIO
def fuc(you):    
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
