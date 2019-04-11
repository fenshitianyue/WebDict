#include "index.h"

int main(int argc, char* argv[]) {
  std::string filename = "./formatted_data";
  InitApp(argc, argv);
  Index* index = Index::Instance();
  index->Build(filename);
  //index->PrintForward();
  index->PrintInverted();

  return 0;
}

