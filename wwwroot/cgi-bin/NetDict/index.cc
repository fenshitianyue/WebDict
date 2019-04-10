#include "index.h"

Index* Index::_inst = NULL;

const word_info* Index::GetWordInfo(const uint64_t id) const {
  if(id > _sequence_index.size()){
    return NULL;
  }
  return &_sequence_index[id];
}

const InvertedList* Index::GetInvertedList(const std::string& key) const {
  auto it = _inverted_index.find(key);
  if(it == _inverted_index.end()){
    return NULL;
  }
  return &(it->second);
}

bool Index::Build(const std::string& file_path){
  //建立正排索引
  //步骤：
  //1.按行读取文件
  std::ifstream file(file_path.c_str());
  if(!file.is_open()){
    return false;
  }
  std::string line;
  while(std::getline(file, line)){
    //2.针对读取的一行数据，构造 word_info 结构
    //3.更新正排索引数据
    const word_info* WordInfo = BuildSequence(line);
    //4.建立倒排索引
    BuildInverted(*WordInfo);
  }
  SortInvertedList();
  file.close();
  return true; 
}

const word_info* Index::BuildSequence(const std::string& line){
  std::vector<std::string> tokens;
  StringUtil::Split(line, &tokens, "\3"); 
  word_info WordInfo;
  WordInfo._id = _sequence_index.size();
  WordInfo._word = tokens[0];
  WordInfo._explain = tokens[1];
  //对单词中文含义进行分词
  
  _sequence_index.push_back(WordInfo);  
  return &_sequence_index.back();
}

void Index::BuildInverted(const word_info& WordInfo){

}
