#!/usr/bin/python
# coding:UTF-8

import urllib2

query_string = r'%E6%B5%8B%E8%AF%95'

# print query_string.decode('utf-8')
print urllib2.unquote(query_string).decode("utf-8")

