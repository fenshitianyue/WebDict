#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace std;

void Split(const string& input, vector<string>* output, const string& split_char){
  boost::split(*output, input, boost::is_any_of(split_char), boost::token_compress_off);
}

void formatted(ofstream& fp, const string& line){
  vector<string> tokens;
  Split(line, &tokens, "\3");
  fp << tokens[0] << endl;
  fp << tokens[1] << endl;
  fp << "-----------------------" << endl;
}

int main() {
  string file_name = "./formatted_data";
  ifstream file(file_name.c_str());
  if(!file.is_open()){
    cout << "词典数据文件打开失败" << endl;
    exit(EXIT_FAILURE);
  }
  ofstream fp("./test_data");
  if(!fp.is_open()){
    cout << "测试数据文件打开失败" << endl;
    exit(EXIT_FAILURE);
  }
  string line;
  while(getline(file, line)){
    formatted(fp, line); 
  }
  fp.close();   
  file.close();
  return 0;
}
