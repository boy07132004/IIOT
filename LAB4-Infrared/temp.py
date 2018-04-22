import RPi.GPIO as GPIO
import time
import sys
import json


# -----------------------DEF------------------------#
def initEnv(pin):
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(pin, GPIO.IN)


'''def initPin(lp):
    GPIO.setup(lp, GPIO.OUT)
'''


def initPin(pins):
    GPIO.setup(pins,GPIO.OUT)
    t_pwm = GPIO.PWM(pins, 1000)
    t_pwm.start(50)
    return t_pwm


def setPin(pwms, duties):
    pwms.ChangeDutyCycle(duties)

def endEnv():
    GPIO.cleanup()


def getSignal(pin):
    start, stop = 0, 0
    signals = []

    while True:
        while GPIO.input(pin) == 0:
            None

        start = time.time()

        while GPIO.input(pin) == 1:
            stop = time.time()
            duringUp = stop - start
            if duringUp > 0.1 and len(signals) > 0:
                return signals[1:]

        signals.append(duringUp)


def compairSignal(s1, s2, rang):
    min_len = min(len(s1), len(s2))

    for i in range(min_len):
        if abs(s1[i] - s2[i]) > rang:
            return False

    return True


def decodeSignal(s, signal_map, rang):
    for name in signal_map.keys():
        if compairSignal(s, signal_map[name], rang):
            return name

    return None


# -----------------------MAIN------------------------#
def main():
    # read argv
    p1 = sys.argv[1]
    l1 = sys.argv[2]
    m1 = sys.argv[3]
    PIN = int(p1[1:len(p1) - 1])
    led = int(l1[1:len(l1) - 1])
    smap = m1[1:len(m1) - 1]

    # open map
    src = open(smap, 'r')
    signal_map = json.loads(src.read())
    src.close()

    # init PIN
    initEnv(PIN)
    #initPin(led)
    

    # function
    print("1:setup led\n0:setoff led\n Power:power off\nup:bright led\ndown:dim led")
    a=1
    # start .........
    while a>0:
        s = getSignal(PIN)
        sig = decodeSignal(s, signal_map, 0.001)
        pwms=initPin(led)

        if sig == str('on'):
            shining=True

        else:print('not record\n')

        while sig == :
            s = getSignal(PIN)
            sig = decodeSignal(s,signal_map,0.001)
            if sig == 
            elif sig == str('up'):
                duties+=10
                setPin(pwms,duties)
            elif sig == str('down') and duties>1:
                duties-=10
                setPin(pwms,duties)
            elif sig == str('off'):
                duties=0
                setPin(pwms,duties)
            
            elif sig == str('power'):
                print('--Good  Bye--')
                a=-1
                     

    # end
    pwms.stop()
    endEnv()


if __name__ == "__main__":
    main()
