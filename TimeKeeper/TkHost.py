#!/usr/bin/env python
from __future__ import print_function, unicode_literals, with_statement

import io, sys, os
import serial
from datetime import datetime

class TkHost(object):
    

    def __init__(self, port):
        ser = serial.Serial(port, 115200, timeout=10)
        totTask = 0.0
        totIntr = 0.0
        totOther = 0.0
        lastX = '';

    def poll(self):
        line = ser.readline()   # read a '\n' terminated line
        # parse message ?
        return line
    
    def __del__(self):
        ser.close()
        
if __name__ == '__main__':
    tk = TkHost('com25')