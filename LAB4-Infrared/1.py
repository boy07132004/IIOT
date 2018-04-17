import RPi.GPIO as GPIO
import time
import sys
import json

def initEnv(pin):
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(pin, GPIO.IN)

def initPin(lp):
    GPIO.setup(lp, GPIO.OUT)

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

def decodeSingal(s, signal_map, rang):
    for name in signal_map.keys():
        if compairSignal(s, signal_map[name], rang):
            return name

    return None

    

def main():
    PIN = int(sys.argv[1])
    led = int(sys.argv[2])
    SIGNAL_MAP = sys.argv[3]
        
    src = open(SIGNAL_MAP, 'r')
    signal_map = json.loads(src.read())
    src.close()

    initEnv(PIN)
    initPin(led)
    while True:
        print("1:setup led\n   0:setoff led\n P:poweroff\n")
        s = getSignal(PIN)
        sig=decodeSingal(s,signal_map,0.001)
        
        print('\n',sig)
        if sig == str('on'):
            GPIO.output(led,True)
        elif sig == str('off'):
            GPIO.output(led,False)
        elif sig == str('power'):break
        else:print('not record')
    endEnv()    
if __name__ == "__main__":
    main()
