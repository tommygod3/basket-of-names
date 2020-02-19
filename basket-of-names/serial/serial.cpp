#include "serial.h"

Serial::Serial(std::string filename)
{
    this->filename = filename;
    this->setInputList(filename);
    this->createResult();
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

void Serial::createResult()
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
    std::list<std::pair<int, Name>> G;
    // Number of names
    const int N = this->input.size() + 1;

    // Print and reset its
    while (firstIter != firstSorted.end() && secondIter != secondSorted.end())
    {
        std::cout << firstIter->first << " " << firstIter->second << "      " << secondIter->first << " " << secondIter->second << std::endl;
        firstIter++;
        secondIter++;
    }
    firstIter = firstSorted.begin();
    secondIter = secondSorted.begin();

    // Iterate through both sorted lists at the same time and add to F and G
    while (firstIter != firstSorted.end() && secondIter != secondSorted.end())
    {
        if (firstIter->first == secondIter->second)
            F.push_back(std::make_pair(firstIter->second, secondIter->first));
        else
        {
            firstIter++;//THIS IS MISSING FIRST ONE
            if (firstIter->first == secondIter->second)
                F.push_back(std::make_pair(firstIter->second, secondIter->first));
            else
            {
                firstIter--;
                G.push_back(std::make_pair(N-1, secondIter->first));
                G.push_back(std::make_pair(N, secondIter->second));
                secondIter++;
                if (firstIter->first == secondIter->second)
                    F.push_back(std::make_pair(firstIter->second, secondIter->first));
            }
        }
        firstIter++;
        secondIter++;
    }


    std::cout << "F:" << std::endl;
    for (auto i : F)
    {
        std::cout << i.first << " " << i.second << std::endl;
    }

    std::cout << "G:" << std::endl;
    for (auto i : G)
    {
        std::cout << i.first << " " << i.second << std::endl;
    }

    // Repeat inductively
    NameList FDash, H;
    std::list<std::pair<int, Name>> GDash;
    unsigned int t = 2;
    while (t < N)
    {
        H = F;
        H.sort();
        F.sort([](const NamePair &a, const NamePair &b) {return a.second < b.second;});
        G.sort([](const std::pair<int, Name> &a, const std::pair<int, Name> &b) {return a.second < b.second;});

        NameList::iterator FIter = F.begin();
        std::list<std::pair<int, Name>>::iterator GIter = G.begin();
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

        GDash.sort([](const std::pair<int, Name> &a, const std::pair<int, Name> &b) {return a.second < b.second;});

        for (std::pair<int, Name> indexedName : G)
        {
            GDash.push_back(indexedName);
        }

        t = 2 * t;
        F = FDash;
        G = GDash;

        FDash.clear();
        GDash.clear();

    }

    G.sort();

    for (std::pair<int, Name> indexedName : G)
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
