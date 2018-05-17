import RPi.GPIO as GPIO
def fuc(you):    
    you=you%4
    if you==1:
        print('r')
    elif you==2:
        print('g')
    elif you==3:
        print('b')
    else:print('close')
