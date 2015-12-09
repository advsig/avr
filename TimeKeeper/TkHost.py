#!/usr/bin/env python
from __future__ import print_function, unicode_literals, with_statement

import io, sys, os
import serial
from datetime import datetime
import time
from codecs import decode, encode

class TkHost(object):
    
    def __init__(self, port):
        self.ser = serial.Serial(port, 115200, timeout=10)
        self.totTask = 0.0
        self.totIntr = 0.0
        self.totOther = 0.0
        self.lastX = '';

    def poll(self):
        line = self.ser.readline()   # read a '\n' terminated line
        # parse message ?
        return line
    
    def write(self, str):
        self.ser.write(encode(str, "ascii"))
        
    def __del__(self):
        self.ser.close()
        
if __name__ == '__main__':
    #tk = TkHost('com26');
    tstr = "T" + str(int(time.time()))
    #tk.write(tstr)
    print(tstr)
  