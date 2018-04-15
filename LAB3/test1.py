import RPi.GPIO as GPIO
import sys
import json
import time

def initEnv():
    GPIO.setmode(GPIO.BOARD)

def initPin(pin):
    GPIO.setup(pin,GPIO.OUT)

def startVoice(pin):
    v = GPIO.PWM(pin,50)
    v.start(50)
    return v

def mkVoice(v,f,d):
    if f>0:
        v.ChangeFrequency(f)
    time.sleep(d)	

def endVoice(v):
    v.stop()

def main():

    P    = sys.argv[1]
    PIN  = int(P[1:len(P)-1])
    J    = sys.argv[2]
    Json = J[1:len(J)-1]

    initEnv()
    

    with open(Json,'r') as file:
        data = json.load(file)

    f=[]
    d=[]
    for i in data:
        f.append(int(i[0]))
        d.append(float(i[1]))	
    initPin(PIN)
    v = startVoice(PIN)
    for j in range(len(f)):
        mkVoice(v,f[j],d[j])


    endVoice(v)
    GPIO.cleanup()

if __name__ == "__main__":
    main()
