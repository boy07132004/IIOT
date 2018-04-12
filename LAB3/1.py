import time
import sys
import Adafruit_DHT as DHT
import os


def main():
    P       =sys.argv[1]
    BCM_PIN =int(P[1:len(P)-1])
    co      =sys.argv[2]
    cond    =co[1:len(co)-1]
    mt      =sys.argv[3]
    monit_t =float(mt[1:len(mt)-1])
    cb      =sys.argv[4]
    callback=cb[1:len(cb)-1]
    
    run = int(monit_t / 5)
    cc  = os.popen(callback).read()
    a   =cc.split("\n")

    for i in range(run) :   
        h, t = DHT.read_retry(11, BCM_PIN)
        h=h/100
        wh={'t':t,'h':h}
        if eval(cond,{"__builtins__":None},wh):
            print(a)

        else:break
        time.sleep(5)
    

if __name__ == "__main__":
    main()
