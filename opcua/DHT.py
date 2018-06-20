import Adafruit_DHT as DHT


def H():
    h=0
    t=0
    h, t = DHT.read_retry(11, 18)
    return float(h)
def M():
    h=0
    t=0
    h, t = DHT.read_retry(11, 18)
    return float(t)
