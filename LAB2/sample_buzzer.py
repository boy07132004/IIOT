#!/usr/bin/env python3
"""
This is a sample program for buzzer on RaspberryPi(R).

@author: FATESAIKOU
@argv[1]: buzzer input pin
@argv[2]: length of each voice(delay)
@argv[3]: voice number of total
"""

import RPi.GPIO as GPIO
import time
import sys
import numpy as np

def initEnv():
    GPIO.setmode(GPIO.BOARD)

def initPin(pin):
    GPIO.setup(pin, GPIO.OUT)

def startVoice(pin):
    v = GPIO.PWM(pin, 50)
    v.start(50)
    return v

def mkVoice(v, f, d):
    v.ChangeFrequency(f)
    time.sleep(d)

def endVoice(v):
    v.stop()

def getFreqs(freq_num):
    cand_freqs = [
        261, # Do
        277, # Do#
        293, # Re#
        311, # Re#
        329, # Mi
        349, # Fa
        369, # Fa#
        392, # Sol
        415, # Sol#
        440, # La
        466, # La#
        493, # Si
        523  # Do
    ]

    return np.random.choice(cand_freqs, freq_num)

def main():
    print("Env Init")
    PIN      = int(sys.argv[1])
    DELAY    = float(sys.argv[2])
    FREQ_NUM = int(sys.argv[3])
    initEnv()

    print("Get Freq")
    freqs = getFreqs(FREQ_NUM)

    print("GPIO Init")
    initPin(PIN)
    v = startVoice(PIN)
    
    print("Playing")
    for f in freqs:
        if f > 0:
            mkVoice(v, f, DELAY)

    print("Ending")
    endVoice(v)

    GPIO.cleanup()

if __name__ == "__main__":
    main()
