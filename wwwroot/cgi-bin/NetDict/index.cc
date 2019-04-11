#include "index.h"

DEFINE_string(dict_path, "/home/zanda/third_part/data/jieba_dict/jieba.dict.utf8", "词典路径");
DEFINE_string(hmm_path, "/home/zanda/third_part/data/jieba_dict/hmm_model.utf8", "hmm 词典路径");
DEFINE_string(user_dict_path, "/home/zanda/third_part/data/jieba_dict/user.dict.utf8", "用户词典路径");
DEFINE_string(idf_path, "/home/zanda/third_part/data/jieba_dict/idf.utf8", "idf 词典路径");
DEFINE_string(stop_word_path, "/home/zanda/third_part/data/jieba_dict/stop_words.utf8", "暂停词词典路径");

Index* Index::inst_ = NULL;
Index::Index() : jieba_(FLAGS_dict_path,
                        FLAGS_hmm_path,
                        FLAGS_user_dict_path,
                        FLAGS_idf_path,
                        FLAGS_stop_word_path){
  stop_word_dict_.Load(FLAGS_stop_word_path); //加载暂停词到内存，用于后面过滤分词结果中的暂停词
}


bool Index::Build(const std::string& file_path){
  std::ifstream file(file_path.c_str());
  if(!file.is_open()){
    return false;
  }
  std::string line;
  while(std::getline(file, line)){
    const WordInfo* word_info = BuildForward(line);
    BuildInverted(*word_info);
  }
  SortInvertedList();
  file.close();
  return true;
}

const WordInfo* Index::BuildForward(const std::string& line){
  std::vector<std::string> tokens;  
  StringUtil::Split(line, &tokens, "\3");
  WordInfo word_info;
  word_info.word_id = forward_index_.size();
  word_info.word = tokens[0];
  word_info.explain = tokens[1];

  SplitExplain(tokens[1], &word_info); //对单词中文含义进行分词

  forward_index_.push_back(word_info);
  return &forward_index_.back();
}

// void Index::PrintForward(){
//   std::ofstream fp("test_data");
//   if(!fp.is_open()){
//     std::cout << "测试数据文件打开失败" << std::endl;
//     return;
//   }
//   for(const auto& word : forward_index_){
//     fp << word.word_id << "." << word.word << "\3" << word.explain << std::endl;
//     fp << "---------------------------" << std::endl;
//     for(const auto& cut_word : word.cut_result){
//       fp << cut_word << "|";
//     }
//     fp << std::endl;
//   }
//   fp.close();
// }

void Index::SplitExplain(const std::string& explain, WordInfo* word_info){
  std::vector<cppjieba::Word> words; 
  jieba_.CutForSearch(explain, words); //此语句执行完后，words内放置的是包含暂停词的分词结果
  for(const auto& it : words){
    //过滤掉暂停词
    if(stop_word_dict_.Find(it.word)){
      continue;
    }
    //将分词结果绑定到 word_info 中
    word_info->cut_result.push_back(it.word);  
  }
}

void Index::BuildInverted(const WordInfo& word_info){
  //遍历分词结果，取分词结果中的词去倒排索引中查找
  for(const std::string& word : word_info.cut_result){
    InvertedList& inverted_list = inverted_index_[word];
    Weight weight;
    weight.word_id = word_info.word_id;
    weight.weight = CalcWeight(word_info.explain, word);
    inverted_list.push_back(weight);
  } 
}

// void Index::PrintInverted(){
//   std::ofstream fp("test_data");
//   if(!fp.is_open()){
//     std::cout << "测试数据文件打开失败" << std::endl;
//     return;
//   }
//   for(const auto& cut_word : inverted_index_){
//     fp << cut_word.first << std::endl; 
//     for(const auto& it : cut_word.second){
//       fp << "(id:" << it.word_id << " | " << "weight:" << it.weight << ")";
//     }
//     fp << std::endl;
//   }
//   fp.close();
// }

int32_t Index::CalcWeight(const std::string& explain, const std::string& word){
  //这里的算法可能有问题，因为(单词长度/含义长度)长度小于1，用整数表示结果都为0
  //先考虑用(含义长度/单词长度)计算权值
  //TODO
  if(0 == word.size()){
    return 0;
  }
  return explain.size()/word.size();
}

void Index::SortInvertedList(){
  for(auto& inverted_pair : inverted_index_){
    InvertedList& inverted_list = inverted_pair.second;
    //在正式排序之前先进行一个去重
    //然后将重复的doc_id权重增加，提高精准度
    //如果同一个单词的含义的分词结果中包含两个相同的分词，那么他们产生的 Weight 也是相同的
    //又因为在找出重复的分词时，同时要改变重复分词的权重，所以要定义unique的一元谓词
    std::sort(inverted_list.begin(), inverted_list.end(), CmpWeight);
    auto iter = std::unique(inverted_list.begin(), inverted_list.end(), EqualWeight);
    inverted_list.erase(iter, inverted_list.end());
    //正式排序
    std::sort(inverted_list.begin(), inverted_list.end(), CmpWeight);
  }
}

bool Index::EqualWeight(Weight& w1, Weight& w2){
  if(w1.word_id == w1.word_id && w1.weight == w2.weight){
    w1.weight += 1; //找到重复的分词结果后，增加其权重，暂且+1，后面根据具体数据再做更改
    w2.weight += 1;
    return true;
  }
  return false;
}

bool Index::CmpWeight(const Weight& w1, const Weight& w2){
  return w1.weight > w2.weight;
}

bool Index::Save(const std::string& ouput_path){

}

bool Index::Load(const std::string& output_path){
//加载正排索引
//加载倒排索引
}
