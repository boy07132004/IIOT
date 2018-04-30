#!/usr/bin/env python3
"""
This program is a simple rfcomm client to connect to the robotic equipped
with a HC05 module, please check your BlueTooth connection before
starting your testing.

About limits:
    M1(base): [0, 180]
    M2(shoulder): [15, 165]
    M3(elbow): [0, 180]
    M4(vertical wrist): [0, 180]
    M5(rotatory wrist): [0, 180]
    M6(gripper): [10, 73]

@author: FATESAIKOU
@argv[1]: robotic_addr # 98:D3:32:30:57:79
@argv[2]: home_action  # 0000000100100100095050
"""

import time
import math
import sys

from bluetooth import *

ROBOTIC_ADDR = sys.argv[1]
HOME = sys.argv[2]

def genAction(action):
    print(action)
    result = "0000"
    for a in action:
        result += str(a).zfill(3)

    return result

def main():
    client_socket = BluetoothSocket( RFCOMM )
    client_socket.connect((ROBOTIC_ADDR, 1))

    client_socket.send(HOME)
    print(client_socket.recv(1024))
    time.sleep(5)

    for j in range(1, 50):
        client_socket.send(genAction([
            0, 100, 90, 0, 0, 0]))
        time.sleep(2)
        for i in [i for i in range(1, 180, j)] + [i for i in range(180, 0, -j)]:
            client_socket.send(genAction([
                0,                            # M1
                100,                          # M2
                90 + int((i - 90) / 3),       # M3
                i,                            # M4
                i,                            # M5
                36 + abs(int((i % 73) - 36)), # M6
            ]))
            time.sleep(0.05*math.sqrt(j))

    print("Finished")

    client_socket.close()

if __name__ == "__main__":
    main()
