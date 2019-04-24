#!/usr/bin/python
# -*- coding: utf-8 -*-

import httplib
import md5
import urllib
import random
import chardet
import json # 调用json解析词典API响应

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
    result = _result['trans_result']
    print result[0]['dst']

    #trans_word = result[0]['dst']
    #print trans_word

    if httpClient:
        httpClient.close()

if __name__ == "__main__":
    query = "world"
    coding = chardet.detect(query[0])
    if coding['encoding'] == "ascii":
        manage(query, 'en', 'zh')
    else:
        manage(query, 'zh', 'en')

