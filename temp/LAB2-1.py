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

'''def getFreqs(freq_num):
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
'''
def main():
    
    y = sys.argv[1]
    y1 = y.split("[")
    y2 = y1[1].split("]")
    pin=(int(y2[0]))
    freq = []
    x = sys.argv[2]
    x1 = x.split("[")
    x2 = x1[1].split("]")
    x3 = x2[0].split(":")
    for s in x3[0:]:
        freq.append(int(s))

    initEnv()
    initPin(pin)

    v = startVoice(pin)
    for f in freq:
        if f > 0:
            mkVoice(v, f, 5)

    endVoice(v)

    GPIO.cleanup()


if __name__ == "__main__":
    main()
