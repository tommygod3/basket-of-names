#include <iostream>
#include <chrono>
#include <utility>
#include <unordered_map>
#include <filesystem>
#include <fstream>

#include "serial.h"

// Timing code from NTU:

using std::chrono::steady_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;


/* timingTestMap(n) creates a std::map<int,string>, and then
 * performs n random insertions followed by n random lookups.
 * The time taken (in milliseconds) for the lookups is returned.
 */
milliseconds timingTest(std::string filename)
{
    Serial serial = Serial(filename);
    
    steady_clock::time_point startTime = steady_clock::now();

    serial.createResults();

    steady_clock::time_point finishTime = steady_clock::now();

    return duration_cast<milliseconds>(finishTime - startTime);
}

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    if (argc == 2)
    {
        std::unordered_map<std::string, milliseconds> results;
        
        std::list<std::string> filenames;

        fs::path testDataPath {"basket-of-names/test-data/"};



        for(auto& p: fs::recursive_directory_iterator(testDataPath))
        {
            if (p.path().string().find("input") != std::string::npos)
            {
                std::cout << p.path() << '\n';
                std::string filename = p.path().string();
                results.insert(std::make_pair(filename, timingTest(filename)));
            }
        }

        std::ofstream outfile;
        outfile.open("serial-results.csv");
        outfile << "filename,ms" << "\n";
        for (auto & item : results)
        {
            outfile << item.first;
            outfile << ",";
            outfile << std::to_string(item.second.count());
            outfile << "\n";
        }

    }
}
