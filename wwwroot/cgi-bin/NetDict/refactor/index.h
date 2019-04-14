#pragma once 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
//#include <boost/algorithm/string.hpp>
#include <cppjieba/Jieba.hpp>
//#include <gflags/gflags.h>
#include "util.hpp"


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
  bool Save(const std::string& before_output_path, const std::string& after_output_path);
  bool Load(const std::string& before_intput_path, const std::string& after_input_path);
  void FindChinese(const std::string& word);
  ///* test interface 
  void PrintForward();
  void PrintInverted();
  //the end */ 
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


