#!/usr/bin/python
# coding:UTF-8

import urllib2
import chardet

query_string = r'%E6%B5%8B%E8%AF%95'

# print query_string.decode('utf-8')
string = urllib2.unquote(query_string).decode("utf-8")

string = string.encode("utf-8")
coding = chardet.detect(string)

if coding['encoding'] == 'ascii':
    print "detect successful"
else:
    print string
