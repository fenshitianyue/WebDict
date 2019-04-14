#include "index.h"

int main(int argc, char* argv[]) {
  std::string filename = "./formatted_data";  
  std::string before_output = "./forward_index";
  std::string after_output = "./inverted_index";
  InitApp(argc, argv);
  Index* index = Index::Instance();
  index->Build(filename);
  index->Save(before_output, after_output);
  return 0;
}
