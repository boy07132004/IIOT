import time
import sys
import Adafruit_DHT as DHT


def main():
    
    h, t = DHT.read_retry(11, BCM_PIN)
    h=h/100


if __name__ == "__main__":
    main()
