#include "serial.h"

Serial::Serial(std::string filename)
{
    this->filename = filename;
    this->setInputList(filename);
}

void Serial::setInputList(std::string filename)
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

        this->input.push_back(std::make_pair(first, second));
    }
}

void Serial::createResults()
{
    NameList firstSorted = this->input;
    firstSorted.sort();

    NameList secondSorted = this->input;
    secondSorted.sort([](const NamePair &a, const NamePair &b) {return a.second < b.second;});

    NameList::iterator firstIter = firstSorted.begin();
    NameList::iterator secondIter = secondSorted.begin();

    // (xi, xi+t)
    NameList F;
    // (i, xi)
    std::list<std::pair<unsigned long, Name>> G;
    // Number of names
    const unsigned long N = this->input.size() + 1;

    firstIter = firstSorted.begin();
    secondIter = secondSorted.begin();

    // Iterate through both sorted lists at the same time and add to F and G
    while (firstIter != firstSorted.end() && secondIter != secondSorted.end())
    {
        if (firstIter->first == secondIter->second)
            F.push_back(std::make_pair(secondIter->first, firstIter->second));
        else
        {
            
            firstIter++;
            if (firstIter->first == secondIter->second)
            {
                F.push_back(std::make_pair(secondIter->first, firstIter->second));
            }
            else
            {
                firstIter--;
                G.push_back(std::make_pair(N-1, secondIter->first));
                G.push_back(std::make_pair(N, secondIter->second));
                secondIter++;
                if (firstIter->first == secondIter->second)
                    F.push_back(std::make_pair(secondIter->first, firstIter->second));
            }
        }
        firstIter++;
        secondIter++;
    }

    // Repeat inductively
    NameList FDash, H;
    std::list<std::pair<unsigned long, Name>> GDash;
    unsigned long t = 2;
    G.sort([](const std::pair<unsigned long, Name> &a, const std::pair<unsigned long, Name> &b) {return a.second < b.second;});
    while (t < N)
    {
        H = F;
        H.sort();
        F.sort([](const NamePair &a, const NamePair &b) {return a.second < b.second;});

        NameList::iterator FIter = F.begin();
        std::list<std::pair<unsigned long, Name>>::iterator GIter = G.begin();
        NameList::iterator HIter = H.begin();


        while (FIter != F.end())
        {
            if (HIter != H.end() && FIter->second == HIter->first)
            {
                FDash.push_back(std::make_pair(FIter->first, HIter->second));
                FIter++;
                HIter++;
            }
            else if (GIter != G.end() && FIter->second == GIter->second)
            {
                GDash.push_back(std::make_pair(GIter->first - t, FIter->first));
                FIter++;
                GIter++;
            }
            else if (GIter != G.end() && FIter->second > GIter->second)
            {
                GIter++;
            }
            else if (HIter != H.end() && FIter->second > HIter->first)
            {
                HIter++;
            }
        }

        GDash.sort([](const std::pair<unsigned long, Name> &a, const std::pair<unsigned long, Name> &b) {return a.second < b.second;});

        GDash.merge(G, [](const std::pair<unsigned long, Name> &a, const std::pair<unsigned long, Name> &b) {return a.second < b.second;});

        t = 2 * t;
        F = FDash;
        G = GDash;

        FDash.clear();
        GDash.clear();

    }

    G.sort();

    for (std::pair<unsigned long, Name> indexedName : G)
    {
        this->result.push_back(indexedName.second);
    }

}

void Serial::printResults()
{
    for (Name name : this->result)
    {
        std::cout << name << "\n";
    }
}
