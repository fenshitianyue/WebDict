#!/usr/bin/python
#coding:UTF-8

import pymysql #数据库
import chardet #中英分流
import ctypes #调用C静态库接口

ll = ctypes.cdll.LoadLibrary
lib = ll("./libcgibase.so")

def Error404():
  print "<h1>服务器怕是偷懒了</h1>"
  print "<br>"
  print "<h3>请联系这个人类，QQ：1262167092</h3>"

def FindEmpty():
    pass

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


def manage_english(cursor, buf):
    sql = "select * from mydict where word = %s" % (buf)
    try:
        cursor.execute(sql)
        if cursor.rowcount == 0:
            FindEmpty()
        results = cursor.fetchall()
        for row in results:
            meaning = row[1]
            Response(meaning)
    except:
        Error404()

def manage_chinese(cursor, buf):
    pass


if __name__ == "__main__":
    db = pymysql.connect("localhost", "root", "nihao", "Dict", charset = "utf8")

    cursor = db.cursor()
#获取页面请求->查询词

    GetQueryStringPy = lib.GetQueryString
    buf = "" * 30 #设置缓冲区容量
    ret = GetQueryStringPy(buf)
    if ret < 0:
        Error404()
        db.close()
#对接收到的数据进行字符串处理
    buf = buf[2:]
#进行中英文分流
    coding = chardet.detect(buf)
    if coding['encoding'] == "ascii":
        manage_english(cursor, buf)
    else:
        manage_chinese(cursor, buf)
    db.close()




