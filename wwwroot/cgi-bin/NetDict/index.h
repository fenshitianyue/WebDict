#pragma once 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <cppjieba/Jieba.hpp>
#include <gflags/gflags.h>

/*             util            */
class DictUtil{
public:
  bool Load(const std::string& file_path){
    std::ifstream file(file_path.c_str());
    if(!file.is_open()){
      return false;
    }
    std::string line;
    while(std::getline(file, line)){
      set_.insert(line);
    }
    file.close();
    return true;
  }
  bool Find(const std::string& key) const {
    return set_.find(key) != set_.end();
  }
private:
  std::unordered_set<std::string> set_;
};

class StringUtil{
public:
  static void Split(const std::string& input, std::vector<std::string>* output, const std::string& split_char){
    boost::split(*output, input, boost::is_any_of(split_char), boost::token_compress_off);
  }
};

static void InitApp(int argc, char** argv){
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

/* end util */

struct WordInfo{
  int64_t word_id;
  std::string word;
  std::string explain;
  //含义的分词结果
  std::vector<std::string> cut_result;
};

struct Weight{
  int64_t word_id;
  int32_t weight;
};

typedef std::vector<Weight> InvertedList;

class Index{
public:
  static Index* Instance(){
    if(inst_ == NULL){
      inst_ = new Index();
    }
    return inst_;
  }
  Index();
  bool Build(const std::string& file_path);
  bool Save(const std::string& output_path);
  bool Load(const std::string& output_path);
  //void PrintForward();
  //void PrintInverted();
private:
  const WordInfo* BuildForward(const std::string& line);  //创建正排索引
  void SplitExplain(const std::string& explain, WordInfo* word_info); //对单词中文含义进行分词
  static bool CmpWeight(const Weight& w1, const Weight& w2);
  static bool EqualWeight(Weight& w1, Weight& w2);
  int32_t CalcWeight(const std::string& explain, const std::string& word); //计算分词的权重
  void BuildInverted(const WordInfo& word_info); //创建倒排索引
  void SortInvertedList(); //对倒排拉链排序
private:
  std::vector<WordInfo> forward_index_;
  std::unordered_map<std::string, InvertedList> inverted_index_;
  cppjieba::Jieba jieba_;
  //暂停词
  DictUtil stop_word_dict_; 
  static Index* inst_;
};


