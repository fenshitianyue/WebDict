#!/bin/bash

# 检测 http 服务器进程是否还在运行
pidof http_server
if [ $? -ne 0 ]
then 
  nohup ../bin/http_server 0.0.0.0 9090 & # 尝试重启 http 服务器
  if [ $? -eq 0 ]
  then 
    echo "Service attempt restart!!!" | mail -s "Service Log" 1262167092@qq.com #使用Linux自带的mail发送邮件
  fi
fi




