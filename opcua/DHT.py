import Adafruit_DHT as DHT


def H(DHT):
    h, t = DHT.read_retry(11, 17)
    return h
def M(DHT):
    h, t = DHT.read_retry(11, 17)
    return t
