import RPi.GPIO as GPIO
import time
import sys
import json

#-----------------------DEF------------------------#
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
#-----------------------DEF------------------------#
def main():
    # read argv
    p1  = sys.argv[1]
    l1  = sys.argv[2]
    m1  = sys.argv[3]
    PIN = int(p1[1:len(p1)-1])
    led = int(l1[1:len(l1)-1])
    smap= m1[1:len(m1)-1]
    
    # open map
    src = open(smap, 'r')
    signal_map = json.loads(src.read())
    src.close()

    # init PIN
    initEnv(PIN)
    initPin(led)
    
    # start .........
    while True:
    
    # function
        print("1:	setup led\n0	:setoff led\nPower:poweroff\n")
        s = getSignal(PIN)
        sig=decodeSingal(s,signal_map,0.001)
        
        if sig == str('on'):GPIO.output(led,True)
        
        elif sig == str('off'):GPIO.output(led,False)
        
        elif sig == str('power'):break
        




        else:print('not record')
    
    # end
    endEnv()

if __name__ == "__main__":
    main()
