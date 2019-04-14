#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <mysql/mysql.h>
#include "index.h"

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


int GetQueryString(char buf[]){
  //1.从环境变量中读取方法
  char *method = getenv("REQUEST_METHOD");
  if(method == NULL){
    //由于标准输出已经被重定向到管道，所以这里将错误信息输出到标准出错
    fprintf(stderr, "getenv REQUEST_METHOD failed!\n");
    return -1;
  }
  //2.如果是GET方法，再从环境变量中 QUERY_STRING
  if(strcasecmp(method, "GET") == 0){
    char *p = getenv("QUERY_STRING");
    if(p == NULL){
      fprintf(stderr, "getenv REQUEST_STRING failed!\n");
      return -1;
    }
    strcpy(buf, p);
    return 0;
  }else{
    //3.如果是POST方法，从环境变量中获取到 Content-Length，再根据Content-Length数值从标准输入中读取数据
    char *p = getenv("CONTENT_LENGTH");
    if(p == NULL){
      fprintf(stderr, "getenv CONTENT_LENGTH failed!\n");
      return -1;
    }
    int content_length = atoi(p);
    int i = 0;
    for(; i < content_length; ++i){
      read(0,&buf[i], 1);
    }
    buf[content_length] = '\0';
  }
 return 0; 
}

int main(int argc, char* argv[]) {
  std::string before_output = "./forward_index";
  std::string after_output = "./inverted_index";
  InitApp(argc, argv);
  Index* index = Index::Instance();
  index->Load(before_output, after_output); //加载索引文件到内存
  //初始化MySQL
  MYSQL sql;
  if(!mysql_real_connect(&sql, "localhost", "root", "nihao.", "Dict" , 3306, NULL, 0)){
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
  //获取页面请求：查询词
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

  //中英查询分流
  if((buf[0] & 0x80) == 0){ //english
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
  }else{ //chinese
    std::string word(buf);
    //TODO
  }
  return 0;
}

