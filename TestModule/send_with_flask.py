#!/usr/bin/python
#coding:UTF-8

from flask import Flask
from flask_mail import Mail,Message
#import requests



app=Flask(__name__)

app.config['MAIL_SERVER'] = 'smtp.qq.com'
app.config['MAIL_SUPPRESS_SEND'] = False
app.config['MAIL_PORT'] = 465
app.config['MAIL_USE_SSL'] = True
app.config['MAIL_USERNAME'] = '1262167092@qq.com'
app.config['MAIL_PASSWORD'] = 'hgukofrpncegjdh'


mail=Mail(app)

msg = Message('Test e-mail', sender='1262167092@qq.com',recipients=['1262167092@qq.com'])
msg.body = u'This is a test e-mail'

with app.app_context():
    mail.send(msg)

