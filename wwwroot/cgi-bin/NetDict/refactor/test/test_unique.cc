#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct word_info{
  int32_t id;
  int32_t weight;
};

bool func(word_info& w1, word_info& w2){
  if(w1.id == w2.id && w1.weight == w2.weight){
    w1.weight += 2;
    w2.weight += 2;
    return true;
  }
  return false;
}

bool cmp(const word_info& w1, const word_info& w2){
  return w1.weight > w2.weight;
}

int main() {
  vector<word_info> ve; 
  for(int32_t i = 1; i <= 3; ++i){
      ve.push_back(word_info{i, i});
  } 
  for(int32_t i = 1; i <= 2; ++i){
      ve.push_back(word_info{i, i});
  } 
  for(const auto& it : ve){
    cout << "id:" << it.id << " | " <<  "weight:" << it.weight << endl;
  }
  cout << "----------------" << endl;
  sort(ve.begin(), ve.end(), cmp); 
  auto iter = unique(ve.begin(), ve.end(), func);
  ve.erase(iter, ve.end());
  for(auto it = ve.begin(); it != ve.end(); ++it){
    cout << "id:" << it->id << " | " <<  "weight:" << it->weight << endl;
  }

  return 0;
}
