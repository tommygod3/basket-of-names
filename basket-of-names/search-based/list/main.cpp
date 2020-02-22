#include "search-based-list.h"


int main(int argc, char *argv[]) {
    if (argc == 2)
    {
        SearchBasedList searchBasedList = SearchBasedList(argv[1]);
        searchBasedList.createResults();
        searchBasedList.printResults();
    }
}
