#!/usr/bin/python
#coding:UTF-8

import pymysql

def manage_english(cursor, buf):
    sql = "select * from mydict where word = %s" % (buf)
    try:
        cursor.execute(sql)
        if cursor.rowcount == 0:
            print "find result is Empty"
        results = cursor.fetchall()
        for row in results:
            meaning = row[1]
            print meaning
    except:
        print "execute faild"

if __name__ == "__main__":
    db = pymysql.connect("localhost", "root", "nihao", "Dict", charset = "utf8")
    cursor = db.cursor()
    buf = "reinterpret"
    manage_english(cursor, buf)

    db.close()






