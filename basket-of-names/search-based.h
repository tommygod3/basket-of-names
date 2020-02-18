#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <utility>


class SearchBased
{
    public:
        using Name = std::string;
        using NameDict = std::map<Name, Name>;
        
        std::string filename;
        std::list<Name> result;
    private:
        NameDict westInput;
        NameDict eastInput;


    public:
        SearchBased(std::string filename);
        void setInputDicts(std::string filename);
        void extendResultWest(Name startingPoint);
        void extendResultEast(Name startingPoint);
        bool isOnEnd(Name name, NameDict& nameDict);
        void printResults();
};
