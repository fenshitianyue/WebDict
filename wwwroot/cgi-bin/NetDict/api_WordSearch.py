#!/usr/bin/python
# -*- coding: utf-8 -*-

import httplib
import md5
import urllib
import urllib2
import random
import chardet
import string
import os
import json # 调用json解析词典API响应
import sys # 调用系统服务
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

def Error404():
    error_content = "服务器可能又偷懒了~\n请联系这个人类，QQ：1262167092"
    Response(error_content)

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
    try:
        httpClient = httplib.HTTPConnection('api.fanyi.baidu.com')
        httpClient.request('GET', myurl)
        response = httpClient.getresponse()
        python_result = json.loads(response)
        explain = python_result['dst']
        Response(explain)
    except Exception:
        print 'http error'
    finally:
        if httpClient:
            httpClient.close()

if __name__ == "__main__":
    query_len = "" * 30
    query = [query_len]
    ret = get_query_string(query)
    if ret > 0:
        query[0] = query[0][2:]
    coding = chardet.detect(query[0])
    if coding['encoding'] == "ascii":
        manage(query, 'en', 'zh')
    else:
        manage(query, 'zh', 'en')

