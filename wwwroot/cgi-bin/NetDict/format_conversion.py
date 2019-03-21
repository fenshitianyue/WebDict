#!/usr/bin/python
# -*- coding: utf-8 -*-

# 这是一份测试将mysql数据库表中数据按照指定格式导入本地文件的代码

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
        sql = "select * from test_dict"
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
    db = pymysql.connect("localhost", "root", "nihao.","test", charset = "utf8")
    cursor = db.cursor()
    find(cursor)
    write_file()
    db.close()
