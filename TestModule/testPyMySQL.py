#!/usr/bin/python
#coding:UTF-8

import pymysql
import chardet

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
    pass

if __name__ == "__main__":
    db = pymysql.connect("localhost", "root", "nihao", "Dict", charset = "utf8")
    cursor = db.cursor()
    buf_tmp = "world"
    buf = [buf_tmp]
    coding = chardet.detect(buf[0])
    if coding['encoding'] == "ascii":
        manage_english(cursor, buf[0])
    else:
        print "查询词是中文"

    db.close()






