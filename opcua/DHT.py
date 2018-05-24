import Adafruit_DHT as DHT


def main(DHT):   
    h, t = DHT.read_retry(11, BCM_PIN)
    return h,t

