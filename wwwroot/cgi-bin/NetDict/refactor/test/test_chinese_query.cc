#include "index.h"

int main(int argc, char* argv[]) {
  InitApp(argc, argv); 
  std::string before_input = "./forward_index";
  std::string after_input = "./inverted_index";
  Index* index = Index::Instance(); 
  index->Load(before_input, after_input);
  std::string result;
  std::string word(argv[1]);
  index->FindChinese(word, result);
  std::cout << result << std::endl;
  return 0;
}

