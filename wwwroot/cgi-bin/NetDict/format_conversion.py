#!/usr/bin/python
# -*- coding: utf-8 -*-

# 从数据库导出词典数据按照指定格式写入文件

import pymysql
import sys
reload(sys)
sys.setdefaultencoding('utf8')

base = {}

def write_file():
    fp = open("/home/zanda/Desktop/PythonCode/formatted_data", "w")
    for word, meaning in base.items():
        fp.write(word + " " + meaning + "\n")
    fp.close()

def find(cursor):
    try:
        sql = "select * from mydict"
        cursor.execute(sql)
        if cursor.rowcount == 0:
            print "查询为空"
        else:
            tmp = cursor.fetchall() # 获取查询的所有结果，即词库
            for row in tmp: #遍历词库，将每一条数据添加到字典base中
                base[row[0]] = row[1]
                # base.update(row[0], row[1]) have bug
    except:
        print "查询失败"


if __name__ == '__main__':
    db = pymysql.connect("localhost", "root", "nihao.","Dict", charset = "utf8")
    cursor = db.cursor()
    find(cursor)
    write_file()
    db.close()
