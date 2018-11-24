# WebDict



### 项目描述

这个项目由三个模块构成：

- 第一个模块为 通用HTTP 服务器框架模块，这个框架是用 C语言实现，可以对浏览器发送的 HTTP 请 求中的 GET 方法和 POST 方法进行响应，静态页面通过封装完整的HTTP响应报文， 读取服务器上的静态资源作为HTTP 响应的body部分然后将HTTP 响应报文发送回浏览器，由浏览器加载；动态页面根据 CGI 协议，创建子进程进行进程替换执行 CGI 模块业务逻辑，父进程读取子进程写入管道的数据作为 body 部分封装完整HTTP 响应报文，然后流程同上。 

- 第二个模块为 CGI模块，这个模块对 HTTP 服务器框架进行二次开发， 使用 python 语言操纵 MySQL数据库进行单词的查询。后台使用缓存，提高查询的效率。

- 第三个模块为 检测模块，这个模块通过 shell 脚本编写，使用 crontab 实现对词典进 程的定时存活检测，使用 linux 的 mail 命令实现对提示邮件的发送，如果词典进程不 存在，由 shell脚本尝试重启进程并且向开发者发送提示邮件。

### 开发环境

操作系统：Centos 7

编译器：gcc-4.8.5

编辑器：vim

### 项目截图



![demon1](fenshitianyue/WebDict/demon/p1.jpg)



![demon2](fenshitianyue/WebDict/demon/p2.jpg)



![demon3](fenshitianyue/WebDict/demon/p3.jpg)
