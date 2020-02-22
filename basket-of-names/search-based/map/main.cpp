#include "search-based.h"


int main(int argc, char *argv[]) {
    if (argc == 2)
    {
        SearchBased searchBased = SearchBased(argv[1]);
        searchBased.createResults();
        searchBased.printResults();
    }
}
