#include <stdio.h>
#include <string.h>
//#include </usr/include/mysql/mysql.h>
#include <mysql/mysql.h>
#include "../cgi_base.h"

//先简化处理一下错误处理页面
void Error404(){
  printf("<h1>服务器怕是偷懒了</h1>");
  printf("<br>");
  printf("<h3>请联系这个人类，QQ：1262167092</h3>");
}


void Response(char *resp){
  printf("<!DOCTYPE HTML>");
  printf("<html>");
  printf("<head>");
  printf("<title>一个简洁的词典</title>");
  printf("<link href=\"css/Response.css\" rel=\"stylesheet\" type=\"text/css\" media=\"all\"/>");
  printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />");
  printf("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1\">");
  printf("<meta name=\"keywords\" content=\"This is a succinct dictionary\" />");
  printf("<link href='http://fonts.googleapis.com/css?family=Open+Sans:300italic,400italic,600italic,700italic,800italic,400,300,600,700,800' rel='stylesheet' type='text/css'>");
  printf("</head>");
  printf("<body>");
  printf("<div class=\"deco\">");
  printf("<textarea class=\"frame_style\" type=\"test\" value=\"Reinterpment\">%s</textarea>", resp);
  printf("</div>");
  printf("</body>");
  printf("</html>");
}

int main() {
  //初始化 MySQL
  MYSQL sql;
  mysql_init(&sql);
  if(!mysql_real_connect(&sql, "localhost", "root", "nihao", "Dict" , 3306, NULL, 0)){
    Error404();
    fprintf(stderr, "MySQL connection failed!\nError Reason:%s\n",mysql_error(&sql));
    mysql_close(&sql);
    return 0;
  }
  //设置 MySQL 默认字符集, 保证浏览器可以显示中文
  if(mysql_set_character_set(&sql, "utf8")) {
    Error404();
    fprintf(stderr, "mysql_set_character_set error:%s", mysql_error(&sql));
    mysql_close(&sql);
    return 0;
  }
  //获取页面请求->查询词
  //目前只考虑查询词是英文的情况
  //TODO
  char buf_old[1024] = {0};
  char buf[1024] = {0};
  int ret = GetQueryString(buf_old);
  if(ret < 0){
    Error404();
    fprintf(stderr, "GetQueryString failed\n");
    mysql_close(&sql);
    return 1;
  }

  //这里需要对接收的数据做一个字符串处理
  //submit提交的数据是这种格式的: 1=query
  //这里采用用空间换时间的方法处理字符串
  strcpy(buf, buf_old + 2);

  //2.进行计算，此处的计算规则是和具体的业务强相关
  //这里的计算主要使用SQL语句来实现
  //通过MySQL对C语言的接口来进行单词的查询
  char search[1024] = {0};
  sprintf(search, "select * from mydict where word = '%s'", buf);
  if(mysql_query(&sql, search)){
    Error404();
    fprintf(stderr, "mysql_query failed!\n");
    mysql_close(&sql);
    return 1;
  }
  //result 保存检索的完整结果
  MYSQL_RES* result = mysql_store_result(&sql); 
  if(NULL == result){
    char error[128] = "不存在这个词，请检查输入！";
    Response(error);
  }else {
    MYSQL_ROW cols = mysql_fetch_row(result);
    //3.拼装成 HTML 页面，写到标准输出 (CGI程序只生成HTTP响应中的body，所以就只需要拼装HTML就OK了)
    Response(cols[1]);
  }
  //收尾工作
  mysql_free_result(result);
  mysql_close(&sql);
  return 0;
}

