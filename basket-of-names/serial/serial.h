#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <utility>
#include <algorithm>


class Serial
{
    public:
        using Name = std::string;
        using NamePair = std::pair<Name, Name>;
        using NameList = std::list<NamePair>;
        
        std::string filename;
        std::list<Name> result;
    private:
        NameList input;
        NameList inputFirstSorted;
        NameList inputSecondSorted;


    public:
        Serial(std::string filename);
        void setInputList(std::string filename);
        void createResult();
        void printResults();
};
