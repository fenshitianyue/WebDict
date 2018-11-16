#pragma once
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//header-only
//C++:用inline替换static
static int GetQueryString(char buf[]){
  //1.从环境变量中读取方法
  char *method = getenv("REQUEST_METHOD");
  if(method == NULL){
    //由于标准输出已经被重定向到管道，所以这里将错误信息输出到标准出错
    fprintf(stderr, "getenv REQUEST_METHOD failed!\n");
    return -1;
  }
  if(strcasecmp(method, "GET") == 0){
    //2.如果是GET方法，再从环境变量中 QUERY_STRING
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
