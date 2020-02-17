#include <string>

#include "dictionary.h"

int main()
{
  using Dict = Containers::Dictionary;

  Dict::Key  key  = std::string("4");
  Dict::Item item = std::string("Matilda");

  Dict dict;
  bool insertSuccess  = dict.insert(key,item);
  Dict::Item* itemPtr = dict.lookup(key);
  bool removeSuccess  = dict.remove(key);

  return insertSuccess && removeSuccess && itemPtr != nullptr;
}
