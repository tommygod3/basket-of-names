#include <utility>

#include "dictionary.h"

int main()
{
  using Dict = Containers::Dictionary<char,std::pair<int,int>>;

  Dict::Key  key  = 'A';
  Dict::Item item = std::pair<int,int>(0,1);

  Dict dict;
  bool insertSuccess  = dict.insert(key,item);
  Dict::Item* itemPtr = dict.lookup(key);
  bool removeSuccess  = dict.remove(key);

  return insertSuccess && removeSuccess && itemPtr != nullptr;
}
