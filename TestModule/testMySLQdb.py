#!/usr/bin/python

import MySQLdb


db = MySQLdb.connect("localhost", "root", "nihao", "test", charset='utf8')

cursor = db.cursor()

cursor.execute("drop table if exists employee")

sql = """create table employee(
first_name char(20) not null,
last_name char(20),
age int,
sex char(1),
income float
);
"""

cursor.execute(sql)
print "Created table successful"

db.close()
