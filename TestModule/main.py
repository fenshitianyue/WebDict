#!/usr/bin/python

import smtplib

sender = 'from@server.com'
receivers = '1262167092@qq.com'

message = """From: From Person <from@server.com>
To: To Person <1262167092@qq.com>
Subject: SMTP e-mail test

This is a test e-mail message ........
"""

def test_smtp():
    try:
        smtpObj = smtplib.SMTP('localhost')
        smtpObj.sendmail(sender, receivers, message)
        print "Successful send e-mail"
    except smtplib.SMTPException:
        print "unable to send e-mail"

if __name__ == "__main__":
    test_smtp()
