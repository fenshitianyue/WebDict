#!/usr/bin/python
#coding:UTF-8

import ctypes

ll = ctypes.cdll.LoadLibrary
lib = ll("./libtest.so")


def TestLib():
    funcpy = lib.func
    buf = " " * 30 #设置大小
    ret = funcpy(buf)
    if ret < 0:
        print "funpy Error!"
    else:
        print buf



if __name__ == "__main__":
    TestLib()



