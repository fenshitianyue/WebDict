#!/bin/bash 

pidof http_server
if [ $? -eq 0 ]
then 
  # 调用 python 脚本给本人发送邮件提醒
fi
