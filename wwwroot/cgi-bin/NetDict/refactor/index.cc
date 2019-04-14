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

void Index::PrintForward(){
  std::ofstream fp("test_forward");
  if(!fp.is_open()){
    std::cout << "测试数据文件打开失败" << std::endl;
    return;
  }
  for(const auto& word : forward_index_){
    fp << word.word_id << "." << word.word << "\3" << word.explain << std::endl;
    fp << "---------------------------" << std::endl;
    for(const auto& cut_word : word.cut_result){
      fp << cut_word << "|";
    }
    fp << std::endl;
  }
  fp.close();
}
 
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

void Index::PrintInverted(){
  std::ofstream fp("test_inverted");
  if(!fp.is_open()){
    std::cout << "测试数据文件打开失败" << std::endl;
    return;
  }
  for(const auto& cut_word : inverted_index_){
    fp << cut_word.first << std::endl; 
    for(const auto& it : cut_word.second){
      fp << "(id:" << it.word_id << " | " << "weight:" << it.weight << ")";
    }
    fp << std::endl;
  }
  fp.close();
}

//计算权重的算法
//这个函数是中文含义查询单词的核心算法，其逻辑可能会非常复杂。
//这里我暂时用简单的逻辑实现
//后面考虑用 逻辑回归算法 计算单词相关性得出权值
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

//这里和计算权重的算法密切相关
bool Index::EqualWeight(Weight& w1, Weight& w2){
  if(w1.word_id == w1.word_id && w1.weight == w2.weight){
    w1.weight -= 1; //找到重复的分词结果后，减少其权重，暂且-1，后面根据具体数据再做更改
    w2.weight -= 1;
    return true;
  }
  return false;
}

//这个排序逻辑和权值的计算公式密切相关
//由于我权值计算部分的计算公式是：含义长度 / 分词长度，所以相关性越低，得出的权值就越大，所以这里采用升序排序
//即权值越大的，排得越靠后
bool Index::CmpWeight(const Weight& w1, const Weight& w2){
  return w1.weight < w2.weight;
}

bool Index::Save(const std::string& before_ouput_path, const std::string& after_output_path){
  std::ofstream before_fp(before_ouput_path.c_str());
  std::ofstream after_fp(after_output_path.c_str());
  if(!after_fp.is_open() || !before_fp.is_open()){
    std::cout << "Index file save faild" << std::endl;
    return false;
  }
  //保存正排索引
  for(decltype(forward_index_.size()) i = 0; i < forward_index_.size(); ++i){
    before_fp << i //写入 word_id
              << "\3"
              << forward_index_[i].word
              << "\3" 
              << forward_index_[i].explain 
              << "\3" 
              << forward_index_[i].cut_result.size() //加载的时候用于指示每一个正排索引分词后的数目
              << "\3";
    for(const auto& it : forward_index_[i].cut_result){
      before_fp << it 
                << "\3";
    }
    before_fp << std::endl;
  }
  //保存倒排索引
  for(const auto& cur_pair : inverted_index_){
    after_fp << cur_pair.first 
             << "\3"
             << cur_pair.second.size()
             << "\3";
    for(const auto& cut_word : cur_pair.second){
      after_fp << cut_word.word_id 
               << "\3"
               << cut_word.weight
               << "\3";
    }
    after_fp << std::endl;
  } 
  before_fp.close();
  after_fp.close();
  return true;
}

bool Index::Load(const std::string& before_intput_path, const std::string& after_input_path){
  std::ifstream before_fp(before_intput_path.c_str());
  std::ifstream after_fp(after_input_path.c_str());
  if(!after_fp.is_open() || !before_fp.is_open()){
    std::cout << "Index file load faild" << std::endl;
    return false;
  }
  //加载正排索引
  std::string line;
  while(std::getline(before_fp, line)){
    //根据保存正排索引的格式进行分割每一行的字段
    std::vector<std::string> cut_line;
    StringUtil::Split(line, &cut_line, "\3");
    //构造一个WordInfo结构，用分割后的字段分别赋值给WordInfo中的字段
    WordInfo word_info;
    word_info.word_id = atoi(cut_line[0].c_str());
    word_info.word = cut_line[1];
    word_info.explain = cut_line[2];
    std::vector<std::string>::size_type size = atoi(cut_line[3].c_str()); //int size = atoi(cut_line[3].c_str());
    for(std::vector<std::string>::size_type i = 0; i < size; ++i){ 
      word_info.cut_result.push_back(cut_line[4+i]);
    }
    //将构造并赋值完成的WordInfo插入到正排索引中
    forward_index_.push_back(word_info);
  }
  line = ""; //这里考虑一下内存泄漏问题 memory_leak
  //加载倒排索引
  while(std::getline(after_fp, line)){
    //根据保存倒排索引的格式进行分割每一行的字段 
    std::vector<std::string> cut_line;
    StringUtil::Split(line, &cut_line, "\3");
    InvertedList& inverted_list = inverted_index_[cut_line[0]]; //cut_line[0] : word
    Weight weight;
    int size = atoi(cut_line[1].c_str());
    for(int i = 0, j = 0; i < size; ++i, j+=2){
      weight.word_id = atoi(cut_line[2+j].c_str());
      weight.weight = atoi(cut_line[2+(j+1)].c_str());
      inverted_list.push_back(weight);
    }
  } 
  before_fp.close();
  after_fp.close();
  return true;
}


bool Index::FindChinese(const std::string& word, std::string& result){
  auto inverted_list = inverted_index_.find(word);
  if(inverted_list != inverted_index_.end()){
    //目前这里查找汉语对应单词的逻辑是只返回匹配到的倒排索引中的第一个单词
    if(!inverted_list->second.empty()){
      //have BUG
      auto it = std::find_if(forward_index_.begin(), forward_index_.end(), FindArgv(inverted_list->second[0].word_id));
      result = it->word;
    }else{
      return false;
    }
  }else{
    return false;
  }
  return true;
}
