#!/usr/bin/python
#coding:UTF-8

import chardet


t1 = "hello"
t2 = "你好"

test_coding1 = chardet.detect(t1)
test_coding2 = chardet.detect(t2)

if test_coding1['encoding'] == "ascii":
    print "english"
else:
    print "中文"

if test_coding2['encoding'] == "utf-8":
    print "中文"
else:
    print "english"




