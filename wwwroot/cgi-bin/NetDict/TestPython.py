#!/usr/bin/python
#coding:UTF-8

import ctypes
#from ctypes import * #事实上我不知道加上这行代码有什么用

ll = ctypes.cdll.LoadLibrary
lib = ll("./libcgibase.so")

def Error404():
  print "<h1>服务器怕是偷懒了</h1>"
  print "<br>"
  print "<h3>请联系这个人类，QQ：1262167092</h3>"

def Response(resp):
    print "<!DOCTYPE HTML>"
    print "<html>"
    print "<head>"
    print "<title>一个简洁的词典</title>"
    print "<link href=\"css/Response.css\" rel=\"stylesheet\" type=\"text/css\" media=\"all\"/>"
    print "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
    print "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1\">"
    print "<meta name=\"keywords\" content=\"This is a succinct dictionary\" />"
    print "<link href='http://fonts.googleapis.com/css?family=Open+Sans:300italic,400italic,600italic,700italic,800italic,400,300,600,700,800' rel='stylesheet' type='text/css'>"
    print "</head>"
    print "<body>"
    print "<div class=\"deco\">"
    print "<textarea class=\"frame_style\" type=\"test\" value=\"Reinterpment\">%s</textarea>" % resp
    print "</div>"
    print "</body>"
    print "</html>"

#获取查询 query

GetQueryStringPy = lib.GetQueryString

buf = "" * 30 #设置缓冲区容量
ret = GetQueryStringPy(buf)


if __name__ == "__main__":
    pass



