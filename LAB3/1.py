#!/usr/bin/env python3
"""
This program is for RaspberryPi(R) to get DHT11's data with python3.
Notice that you have to install Adafruit_DHT module first with bellow:

    $ git clone https://github.com/adafruit/Adafruit_Python_DHT.git
    $ cd Adafruit_DHT
    $ sudo apt-get update
    $ sudo apt-get install build-essential python-dev
    $ sudo python3 setup.py install

@author: FATESAIKOU
@argv[1]: BCM pin id
@argv[2]: monit time
@argv[3]: monit delay
"""

import time
import sys
import Adafruit_DHT as DHT


def main():
    BCM_PIN    = int(sys.argv[1])
    TOTAL_TIME = float(sys.argv[2])
    DELAY      = float(sys.argv[3])


    iter_num = int(TOTAL_TIME / DELAY)
    for i in range(iter_num):
        h, t = DHT.read_retry(11, BCM_PIN)
        print('Temp={0:0.1f}*C  Humidity={1:0.1f}%'.format(t, h))

        time.sleep(DELAY)

if __name__ == "__main__":
    main()
