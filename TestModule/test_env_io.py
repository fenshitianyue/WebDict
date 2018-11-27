#!/usr/bin/python
#coding:UTF-8

import sys
import string

#num = "5"
#str = sys.stdin.readline(string.atoi(num))
#print str
num = 1

def func(buf):
    buf[0] = "hello world"

buf = " " * 20
tmp = []
tmp.append(buf)
func(tmp)
tmp[0] = tmp[0][6:]
print tmp[0]




