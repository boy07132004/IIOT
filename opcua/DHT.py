import Adafruit_DHT as DHT


def main(DHT):
    h, t = DHT.read_retry(11, 17)
    tur=(h,t)
    return tur
