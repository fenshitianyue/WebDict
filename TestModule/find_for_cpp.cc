#include <iostream>
#include <fstream>
#include <string>
#include <map>


std::map<std::string, std::string> base; //用于保存词库

void split_string(std::string& dst_0, std::string& dst_1, const std::string& src, const char ch){
  std::string::size_type pos;
  pos = src.find_first_of(ch);
  dst_0 = src.substr(0, pos);
  dst_1 = src.substr(pos+1, src.size());
}

void update_data(std::ifstream& fp){
  std::string word;
  std::string explain;
  std::string line;
  ///////////////////////////////////////////////////////
  //test code
  // getline(fp, line); //从词库中读取一行词条
  // split_string(word, explain, line, ' '); //切分当前行词条，提取出当前行数据中有价值的部分
  // std::cout << line << std::endl;
  // std::cout << "word: " << word << std::endl;
  // std::cout << "explain: " << explain << std::endl;
  ///////////////////////////////////////////////////////
  
  while(!fp.eof()){
    getline(fp, line); //从词库中读取一行词条
    split_string(word, explain, line, ' '); //切分当前行词条，提取出当前行数据中有价值的部分
    ///////////////////////////////////////////////////////
    //test code
    // std::cout << word << " " << explain << std::endl;
    ///////////////////////////////////////////////////////
    base.insert(std::pair<std::string, std::string>(word, explain)); //将当前行中提取出的数据添加到数据字典中
  } 
}

int main() {
  std::ifstream fp("formatted_data");
  if(fp.is_open()){
    update_data(fp);
  }
  ///////////////////////////////////////////////////////
  //test code
  //// C++11以前的版本遍历map写法
  //std::map<std::string, std::string>::iterator it;  
  //for(it = base.begin(); it != base.end(); ++it){
  //  std::cout << it->first << " " << it->second << std::endl;
  //}
  //for(const auto& it : base){
  //  std::cout << it.first << " " << it.second << std::endl;
  //}
  ///////////////////////////////////////////////////////
  std::map<std::string, std::string>::iterator it; 
  it = base.find(std::string("zymic"));
  std::string explain;
  if(it != base.end()){
    explain = it->second;
    std::cout << explain << std::endl;
  }else{
    std::cout << "now found!" << std::endl;
  }
  fp.close();
  return 0;
}
