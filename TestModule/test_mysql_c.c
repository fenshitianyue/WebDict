#include <stdio.h>
#include <mysql/mysql.h>

int main(){
  MYSQL sql;
  mysql_init(&sql);
  //printf("MySQL client version: %d \n", mysql_get_client_version());
  if(!mysql_real_connect(&sql, "localhost", "root", "nihao", NULL, 0, NULL, 0)){
    puts("Error connecting!");
    printf("%s\n", mysql_error(&sql));
  }else{
    puts("Succss connecting!");
  }
  char buf[1024] = "reterpret";
  char search[1024] = {0};
  sprintf(search, "select * from mydict where word = '%s'", buf);
  printf("SQL: %s\n", search);

  //if(mysql_query(&sql, search)){
  //  printf("mysql_query failed!\n");
  //}
  return 0;
}
