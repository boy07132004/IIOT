import time
import sys
import Adafruit_DHT as DHT
import subprocess as sp
import logging


logging.basicConfig(level=logging.DEBUG,filename='log.txt', \
format='%(asctime)s -%(name)s -%(message)s')
L = logging.getLogger('STDOUT')
LL= logging.getLogger('STDERR')

def main():
    P       =sys.argv[1]
    BCM_PIN =int(P[1:len(P)-1])
    co      =sys.argv[2]
    cond    =co[1:len(co)-1]
    mt      =sys.argv[3]
    monit_t =float(mt[1:len(mt)-1])
    cb      =sys.argv[4]
    callback=cb[1:len(cb)-1].split()
    run     =int(monit_t / 5)


    #-----whitelist for command-----#
    whitelist=['ls','./0.py']
    #-------------------------------#
    
    for i in range(run) :   
        
        h, t = DHT.read_retry(11, BCM_PIN)
        h=h/100
        wh={'t':t,'h':h}

        if eval(cond,{"__builtins__":None},wh):
            if set(callback)&set(whitelist): 
                out=sp.Popen(callback,stdout=sp.PIPE,stderr=sp.STDOUT)
                (stdout,stderr)=out.communicate()
                L.info(stdout)
            else LL.error(stderr)
        else LL.error(stderr)
        time.sleep(5)


if __name__ == "__main__":
    main()
