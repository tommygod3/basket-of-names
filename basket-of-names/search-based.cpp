#include "search-based.h"

SearchBased::SearchBased(std::string filename)
{
    this->filename = filename;
    this->setInputDicts(filename);
    Name firstName = this->westInput.begin()->first;

    this->extendResultWest(firstName);
    this->extendResultEast(this->eastInput.at(firstName));//TODO what if firstName on far east

    for (auto r : this->result)
    {
        std::cout << r << "\n";
    }

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
    this->result.push_back(name);
    if (!isOnEnd(name, this->westInput))
    {
        extendResultWest(this->westInput.at(name));
    }
}

void SearchBased::extendResultEast(Name name)
{
    this->result.push_front(name);
    if (!isOnEnd(name, this->eastInput))
    {
        extendResultEast(this->eastInput.at(name));
    }
}

bool SearchBased::isOnEnd(Name name, NameDict& nameDict)
{
    return nameDict.find(name) == nameDict.end();
}
