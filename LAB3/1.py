#!/usr/bin/env python3
"""
@author: FATESAIKOU
@argv[1]: BCM pin id
@argv[2]: monit time
@argv[3]: monit delay
"""

import time

import sys

import Adafruit_DHT as DHT


def main():
    P       =sys.argv[1]
    BCM_PIN =int(P[1:len(P)-1])
    co      =sys.argv[2]
    cond    =(co[1:len(co)-1])
    mt      =sys.argv[3]
    monit_t =float(mt[1:len(mt)-1])
    
    run = int(monit_t / 5)
        
    for i in range(run) :   
        h, t = DHT.read_retry(11, BCM_PIN)
        wh={'t':t,'h':h}
        if eval(cond,{"__builtins__":None},wh):
            print('Temp={0:0.1f}*C  Humidity={1:0.1f}%'.format(t, h))
            print("test")
        else:break
        time.sleep(5)
    

if __name__ == "__main__":
    main()
