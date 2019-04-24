#!/usr/bin/python
# -*- coding: utf-8 -*-

import string
import ast
import httplib
import md5
import urllib
import urllib2
import random
import chardet
import os
import sys
reload(sys)
sys.setdefaultencoding('utf8')

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

def get_query_string(buf):
    # 从环境变量中读取方法
    method = os.environ["REQUEST_METHOD"]
    # 如果是GET方法，再从环境变量中读取 QUERY_STRING
    if method.upper() == "GET":
        query_string = os.environ["QUERY_STRING"]
# 将浏览器发送请求字符串从 escape 格式解码为 unicode 格式
        buf[0] = urllib2.unquote(query_string).decode("utf-8")
# 将 unicode 格式的字符串转换为 string 格式的字符串
        buf[0] = buf[0].encode("utf-8")
# 如果是POST方法，从环境变量中获取content-length，再
# 根据其数值从标准输入中读取数据
    else:
        content_length = os.environ["CONTENT_LENGTH"]
        buf[0] = sys.stdin.readline(string.atoi(content_length))
        buf[0] = urllib2.unquote(buf[0]).decode("utf-8")
        buf[0] = buf[0].encode("utf-8")
        buf[0] = buf[0] + "\0"
    return 1

appid = '20190415000288304'
secreKey = '8fWMpQPCuC017jD3fB9R'

url_prefix = '/api/trans/vip/translate'
# formLang = 'en' # 查询词为英文
# toLang = 'zh' # 翻译为中文
salt = random.randint(32768, 65536)

def manage(query, fromLang, toLang):
    sign = appid + query + str(salt) + secreKey
    ml = md5.new()
    ml.update(sign)
    sign = ml.hexdigest()
    myurl = url_prefix+'?appid='+appid+'&q='+urllib.quote(query)+'&from='+fromLang+'&to='+toLang+'&salt='+str(salt)+'&sign='+sign
    httpClient = httplib.HTTPConnection('api.fanyi.baidu.com')
    httpClient.request('GET', myurl)
    response = httpClient.getresponse()
    _result = response.read()
    result = ast.literal_eval(_result)
    result = result['trans_result']
    explain = result[0]['dst']
    explain = explain.decode('unicode_escape')
    Response(explain)

    if httpClient:
        httpClient.close()

if __name__ == "__main__":
    buf_n = " " * 30 #设置接收缓冲去容量
    query = [buf_n]
    ret = get_query_string(query)
    if ret > 0:
      query[0] = query[0][2:]
      coding = chardet.detect(query[0])
      if coding['encoding'] == "ascii":
          manage(query[0], 'en', 'zh')
      else:
          manage(query[0], 'zh', 'en')

