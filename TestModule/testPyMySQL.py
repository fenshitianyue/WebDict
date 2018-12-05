#!/usr/bin/python
#coding:UTF-8

import pymysql
import chardet
import sys
reload(sys)
sys.setdefaultencoding('utf8')

def manage_english(cursor, buf):
    try:
        sql = "select * from mydict where word = '%s'" % (buf)
        print sql
        cursor.execute(sql)
        if cursor.rowcount == 0:
            print "find result is Empty"
        else:
            results = cursor.fetchall()
            for row in results:
                meaning = row[1]
                print meaning
    except:
        print "some error!"

def manage_chinese(cursor, buf):
    sql = "select * from mydict where match(`explain`) against('%s*' in boolean mode)" %(buf)
    cursor.execute(sql)
    if cursor.rowcount == 0:
        print "find result in empty"
    else:
        results = cursor.fetchall()
        result = ""
        for row in results:
            result_line = row[0] + '   ' + row[1] + '\n'
            result = result + result_line
    result = result + '\n\n如果对结果不满意，可以尝试搜索含义相类似的词噢'
    print result

if __name__ == "__main__":
    db = pymysql.connect("localhost", "root", "nihao", "Dict", charset = "utf8")
    cursor = db.cursor()
    buf_tmp = "试验"
    buf = [buf_tmp]
    coding = chardet.detect(buf[0])
    if coding['encoding'] == "ascii":
        manage_english(cursor, buf[0])
    else:
        print "shunt successful"
        manage_chinese(cursor, buf[0])

    db.close()






