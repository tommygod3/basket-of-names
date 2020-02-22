#include "search-based.h"

SearchBased::SearchBased(std::string filename)
{
    this->filename = filename;
    this->setInputDicts(filename);
}

void SearchBased::createResults()
{
    Name firstName = this->westInput.begin()->first;
    this->extendResultWest(firstName);
    if (!isOnEnd(firstName, this->eastInput)) this->extendResultEast(this->eastInput.at(firstName));
}

void SearchBased::setInputDicts(std::string filename)
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

        this->westInput.insert(std::make_pair(first, second));
        this->eastInput.insert(std::make_pair(second, first));
    }
}

void SearchBased::extendResultWest(Name name)
{
    while (!isOnEnd(name, this->westInput))
    {
        this->result.push_back(name);
        name = this->westInput.at(name);
    }
    this->result.push_back(name);
}

void SearchBased::extendResultEast(Name name)
{
    while (!isOnEnd(name, this->eastInput))
    {
        this->result.push_front(name);
        name = this->eastInput.at(name);
    }
    this->result.push_front(name);
}

bool SearchBased::isOnEnd(Name name, NameDict& nameDict)
{
    return nameDict.find(name) == nameDict.end();
}

void SearchBased::printResults()
{
    for (Name name : this->result)
    {
        std::cout << name << "\n";
    }
}
