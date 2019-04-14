#include <iostream>
#include <vector>
#include <algorithm>

struct type{
  int id;
  int weight;
};

bool SortFunc(const type& w1, const type& w2){
  return w1.weight < w2.weight;
}

bool CmpFunc(type& w1, type& w2){
  if(w1.weight == w2.weight){
    w1.weight += 3;
    w2.weight += 3;
    return true;
  }
  return false;
}

int main() {
  std::vector<type> ve;
  for(int i = 1; i <= 3; ++i){
    ve.push_back(type{i, i}); 
  }

  for(int i = 2; i <= 3; ++i){
    ve.push_back(type{i, i});
  }
  std::sort(ve.begin(), ve.end(), SortFunc);
  for(std::vector<type>::iterator it = ve.begin(); it != ve.end(); ++it){
    std::cout << it->id << " | " << it->weight << std::endl;
  }
  std::cout << "--------------" << std::endl;
  auto iter = std::unique(ve.begin(), ve.end(), CmpFunc);
  ve.erase(iter, ve.end());
  std::sort(ve.begin(), ve.end(), SortFunc);
  for(std::vector<type>::iterator it = ve.begin(); it != ve.end(); ++it){
    std::cout << it->id << " | " << it->weight << std::endl;
  }

  return 0;
}
