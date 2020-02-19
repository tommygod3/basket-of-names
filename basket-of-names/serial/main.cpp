#include "serial.h"


int main(int argc, char *argv[]) {
    if (argc == 2)
    {
        Serial(argv[1]).printResults();
    }
}
