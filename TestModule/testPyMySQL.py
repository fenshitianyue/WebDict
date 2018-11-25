#!/usr/bin/python
#coding:UTF-8

import pymysql
import chardet

def manage_english(cursor, buf):
    sql = "select * from mydict where word = '%s'" % (buf)
    cursor.execute(sql)
    if cursor.rowcount == 0:
        print "find result is Empty"
    else:
        results = cursor.fetchall()
        for row in results:
            meaning = row[1]
            print meaning

if __name__ == "__main__":
    db = pymysql.connect("localhost", "root", "nihao", "Dict", charset = "utf8")
    cursor = db.cursor()
    buf = "reinterpret"
    coding = chardet.detect(buf)
    if coding['encoding'] == "ascii":
        manage_english(cursor, buf)
    else:
        print "查询词是中文"

    db.close()






