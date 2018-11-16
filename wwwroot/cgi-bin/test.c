#include <stdio.h>
#include "cgi-base.h" 

int main(){

  char buf[1024] = {0};
  GetQueryString(buf);
  return 0;
}
