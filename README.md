# WebDict



### 项目描述

---

这个项目由三个模块构成：

- 第一个模块为通用HTTP服务器框架模块，这个框架是用C语言实现，可以对浏览器发送的HTTP请求中的GET方法和POST方法进行响应，静态页面通过封装完整的HTTP响应报文，读取服务器上的静态资源作为HTTP响应的body部分然后将HTTP响应报文发送回浏览器，由浏览器加载；动态页面根据CGI协议，创建子进程进行进程替换执行CGI模块业务逻辑，父进程读取子进程写入管道的数据作为body部分封装完整HTTP响应报文，然后流程同上。 
- 第二个模块为CGI模块，这个模块在HTTP服务器框架的基础上进行二次开发。从HTTP服务器框架逻辑中创建的环境变量中读取到浏览器发出的query，判定query的字符集，如果是英文，判定query的长度是否超过指定长度，如果是，则调用Baidu translation API接口进行翻译，否则就调用MySQL接口在数据库中进行查询；如果是中文，则调用Baidu translation API接口进行翻译。得到结果后，再将结果封装成一个HTML页面通过管道发送给HTTP服务器框架。
- 第三个模块为检测模块，这个模块通过shell脚本编写，使用crontab实现对词典进程的定时存活检测，使用linux的 mail命令实现对提示邮件的发送，如果词典进程不存在，由shell脚本尝试重启进程并且向开发者发送提示邮件。

### 开发环境

---

操作系统：Centos 7

编译器：gcc-4.8.5

编辑器：vim

### 项目截图

---



![Image text](https://github.com/fenshitianyue/WebDict/blob/master/demon/p1.jpg)



![Image text](https://github.com/fenshitianyue/WebDict/blob/master/demon/p2.jpg)



![Image text](https://github.com/fenshitianyue/WebDict/blob/master/demon/p3.jpg)
