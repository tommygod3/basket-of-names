#include "search-based-list.h"

SearchBasedList::SearchBasedList(std::string filename)
{
    this->filename = filename;
    this->setInputDicts(filename);
}

void SearchBasedList::createResults()
{
    Name firstName = this->westInput.begin()->first;
    this->extendResultWest(firstName);
    if (!isOnEnd(firstName, this->eastInput)) this->extendResultEast(*findInDict(firstName, eastInput));
}

void SearchBasedList::setInputDicts(std::string filename)
{
    std::ifstream inputFile(filename);
    std::string line;
    std::string first;
    std::string second;
    while (std::getline(inputFile, line))
    {
        std::stringstream lineStream(line);

        std::getline(lineStream, first, ',');
        std::getline(lineStream, second, ',');

        this->westInput.push_back(std::make_pair(first, second));
        this->eastInput.push_back(std::make_pair(second, first));
    }
}

void SearchBasedList::extendResultWest(Name name)
{
    while (!isOnEnd(name, this->westInput))
    {
        this->result.push_back(name);
        name = *findInDict(name, this->westInput);
    }
    this->result.push_back(name);
}

void SearchBasedList::extendResultEast(Name name)
{
    while (!isOnEnd(name, this->eastInput))
    {
        this->result.push_front(name);
        name = *findInDict(name, this->eastInput);
    }
    this->result.push_front(name);
}

bool SearchBasedList::isOnEnd(Name name, NameList& nameDict)
{
    return findInDict(name, nameDict) == nullptr;
}

void SearchBasedList::printResults()
{
    for (Name name : this->result)
    {
        std::cout << name << "\n";
    }
}

SearchBasedList::Name* SearchBasedList::findInDict(Name name, NameList& nameDict)
{
    for (NamePair& names : nameDict)
    {
        if (names.first == name)
        {
            return &names.second;
        }
    }
    return nullptr;
}
