#!/usr/bin/python
# -*- coding: utf-8 -*-

import pymysql  # 数据库
import chardet  # 判断字符集
# import ctypes  # 调用C静态库接口
import os # 调用系统服务
## import re # 在中文缓存中使用模糊匹配
import string # 字符串操作
import sys # 调用系统服务
reload(sys)
sys.setdefaultencoding('utf8')

# ll = ctypes.cdll.LoadLibrary
# lib = ll("../common/libcgibase.so")

# 初步将缓存的容量定为 50 个词

# 设置中英文缓存
EnglishCache = {}
## ChineseCache = {}

def add_english_dict(line):
    list_str = line.split(" ")
    word = list_str[0].strip("\n")
    meaning = list_str[1].strip("\n")
    EnglishCache[word] = meaning

## def add_chinese_dict(line):
##     list_str = line.split(" ")
##     word = list_str[0].strip("\n")
##     meaning = list_str[1].strip("\n")
##     ChineseCache[word] = meaning

def read_cache():
    fp_e = open("/root/YAB/web_dict/wwwroot/cgi-bin/NetDict/cache/EnglishCache", "r")
##    fp_c = open("/root/YAB/web_dict/wwwroot/cgi-bin/NetDict/cache/ChineseCache", "r")
# update english cache
    while True:
        line = fp_e.readline()
        if line:
            add_english_dict(line)
        else:
            break
## # update chinese cache
##     while True:
##         line = fp_c.readline()
##         if line:
##             add_chinese_dict(line)
##         else:
##             break
    fp_e.close()
##    fp_c.close()

def write_english_cache():
    fp = open("/root/YAB/web_dict/wwwroot/cgi-bin/NetDict/cache/EnglishCache", "w")
    num = len(EnglishCache) - 50
# 如果缓存中的元素个数超过指定缓存容量，则随机删除超过容量的元素
    if num > 0:
        for word in EnglishCache.key():
            del EnglishCache[word]
            num = num - 1
            if num == 0:
                break
    for word, meaning in EnglishCache.items():
        fp.write(word + " " + meaning + "\n")
    fp.close()

## def write_chinese_cache():
##     fp = open("cache/ChineseCache", "w")
##     num = len(ChineseCache) - 50
## # 如果缓存中的元素个数超过指定缓存容量，则随机删除超过容量的元素
##     if num > 0:
##         for word in ChineseCache.key():
##             del ChineseCache[word]
##             num = num - 1
##             if num == 0:
##                 break
##     for word, meaning in ChineseCache.items():
##         fp.write(word + " " + meaning + "\n")
##     fp.close()

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

def FindEmpty():
    empty_content = "抱歉，未找到查询词~\n试着换一个近似含义的词搜索。\neg: 试验搜不到，换实验试试~"
    Response(empty_content)

def find_english_cache(buf):
    for word, meaning in EnglishCache.items():
        if word == buf:
            return meaning
    return 'null'

## def find_chinese_cache(buf):
##     pass

def update_english_buf(buf, meaning):
    EnglishCache[buf] = meaning
    write_english_cache()

## def update_chinese_buf(buf, meaning):
##     ChineseCache[buf] = meaning
##     write_chinese_cache()

def manage_english(cursor, buf):
# 从英文缓存中查找
    meaning = find_english_cache(buf)
    if meaning != 'null':
        Response(meaning)
    else:
        try:
            sql = "select * from mydict where word = '%s'" % (buf)
            cursor.execute(sql)
            if cursor.rowcount == 0:
                FindEmpty()
            else:
                results = cursor.fetchall()
                for row in results:
                    meaning = row[1]
                    Response(meaning)
# 更新英文缓存
                update_english_buf(buf, meaning)
        except:
            Error404()

def manage_chinese(cursor, buf):
    try:
        Response(buf)
        sql = "select * from mydict where match(`explain`) against('%s*' in boolean mode)" % (buf)
        cursor.execute(sql)
        if cursor.rowcount == 0:
            # FindEmpty()
            pass
        else:
            results = cursor.fetchall()
            result = ""
            for row in results:
                result_line = row[0] + '    ' + row[1] + '\n'
                result = result + result_line
            result = result + '\n\n如果对结果不满意，可以尝试换一个查询词试试。\neg:实验搜不到，换试验试试~'
            # Response(result)
            pass
    except:
        Error404()

def get_query_string(buf):
    # 从环境变量中读取方法
    method = os.environ["REQUEST_METHOD"]
    # 如果是GET方法，再从环境变量中读取 QUERY_STRING
    if method.upper() == "GET":
        query_string = os.environ["QUERY_STRING"]
        buf[0] = query_string
        Response(unicode(query_string[2:], 'utf-8')) #接收到的query_string的编码可能有问题
# 如果是POST方法，从环境变量中获取content-length，再
# 根据其数值从标准输入中读取数据
    else:
        content_length = os.environ["CONTENT_LENGTH"]
        buf[0] = sys.stdin.readline(string.atoi(content_length))
        buf[0] = buf[0] + "\0"
    return 1

if __name__ == "__main__":
# 将缓存加载到内存
    read_cache()
# 连接数据库
    db = pymysql.connect("localhost", "root", "nihao", "Dict", charset = "utf8")
# 获取控制句柄
    cursor = db.cursor()

# 获取页面请求->查询词
#   GetQueryStringPy = lib.GetQueryString
#   ret = GetQueryStringPy(buf)
    buf_tmp = " " * 30  # 设置缓冲区容量
    buf = [buf_tmp]
# 要在函数内部更改函数外部变量的值，只能传入固定类型的参数，所以这里封装了一层
# 将缓冲区封装到了列表里
    ret = get_query_string(buf)
    if ret < 0:
        # Error404()
        db.close()
# 对接收到的数据进行字符串处理
    buf[0] = buf[0][2:]
# 进行中英文分流
    coding = chardet.detect(buf[0])
    if coding['encoding'] == "ascii":
        # manage_english(cursor, buf[0])
        pass
    else:
        manage_chinese(cursor, buf[0])
# 关闭与数据库的连接
    db.close()

