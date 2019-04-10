#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <boost/algorithm/string.hpp> //使用boost库提供的字符串切分
#include <cppjieba/Jieba.hpp>

using word_info = word_index_proto::WordInfo;
using weight = word_index_proto::Weight;

using InvertedList = std::vector<weight>;

//表示分词结果的统计结果
struct WordCnt{
  int word_cnt;
  WordCnt(): word_cnt(0){}
};


class Index{
public:
  static Index* Instance(){
    if(_inst == NULL){
      _inst = new Index();
    }
    return _inst;
  }
  Index();

private:
  //正排索引
  std::vector<word_info> _sequence_index;
  //倒排索引
  std::unordered_map<std::string, InvertedList> _inverted_index;
  static Index* _inst;
  //cppjieba::Jieba _jieba;
public:
  bool Build(const std::string& file_path);
  const word_info* GetWordInfo(const uint64_t id) const;
  const InvertedList* GetInvertedList(const std::string& key) const;
private:
  //一些用于构建索引的内部函数
  const word_info* BuildSequence(const std::string& line);
  void BuildInverted(const word_info& WordInfo);
  void SortInvertedList();
};

class StringUtil{
public:
  static void Split(const std::string& input, std::vector<std::string>* output, const std::string split_char){
    boost::split(*output, input, boost::is_any_of(split_char), boost::token_compress_off);
  }
};

class Search{

};


