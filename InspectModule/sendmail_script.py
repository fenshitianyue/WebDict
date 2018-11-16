#!/usr/bin/python
#coding:UTF-8

import smtplib
from time import ctime

sender = 'from@Server.com'
receivers = '1262167092@qq.com'
message = """From: From Person <from@Server.com>
To: To Person <1262167092@qq.com>
Subject: Error Log

Http Server will be restart!!!
"""

def send_email():
    "这个函数用于发送服务器异常邮件"
    fo = open("../ErrorLog.txt", "a") #追加到错误日志文件的末尾

    try:
        smtpObj = smtplib.SMTP('localhost')
        smtpObj.sendmail(sender, receivers, message)
        fo.write("Error[%s]: unable to send mail", ctime())
    except smtplib.SMTPException:
        fo.write("Error[%s]: unable to send mail", ctime())
        fo.close()
    else:
        fo.close()

if __name__ == "__main__":
    send_email()

