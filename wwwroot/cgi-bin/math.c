#include"cgi_base.h"


int main() {
  //1.获取到必要的信息，此处需要获取到的信息，就是进行相加的数字
  //  a)如果当前是 GET 请求，就需要解析 QUERY_STRING
  //  b)如果当前是 POST 请求，就需要解析 body
  //  （不管数据是在 QUERY_STRING 中， 还是在 body 中,数据格式都是形如：
  //  a=10&b=20）
  char buf[1024 * 4] = {0};
  //此函数的功能就是自动根据 GET/POST 请求获取出对应的参数信息，并且把参数信息填充到 buf 缓冲区中
  int ret = GetQueryString(buf);
  if(ret < 0){
    fprintf(stderr, "GetQueryString failed\n");
    return 1;
  }
  fprintf(stderr, "buf = %s\n", buf);
  //2.进行计算，此处的计算规则是和具体的业务强相关
  int a, b;
  //sscanf这个函数不推荐使用，因为它错误处理的能力很弱，一旦buf中的格式和指定的格式不同，那么就会出错
  sscanf(buf, "a=%d&b=%d", &a, &b);
  int sum = a + b;
  //3.拼装成 HTML 页面，写到标准输出 （此处由于 CGI 协议规定 CGI程序只生成HTTP响应中的body，所以就只需要拼装HTML就OK了)
  printf("<h1>sum=%d</h1>", sum);
  return 0;
}
