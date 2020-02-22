#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <utility>


class SearchBasedList
{
    public:
        using Name = std::string;
        using NamePair = std::pair<Name, Name>;
        using NameList = std::list<NamePair>;
        
        std::string filename;
        std::list<Name> result;
    private:
        NameList westInput;
        NameList eastInput;


    public:
        SearchBasedList(std::string filename);
        void setInputDicts(std::string filename);
        void createResults();
        void extendResultWest(Name startingPoint);
        void extendResultEast(Name startingPoint);
        bool isOnEnd(Name name, NameList& nameDict);
        void printResults();
        Name* findInDict(Name name, NameList& nameDict);
};
