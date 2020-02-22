#include "serial.h"


int main(int argc, char *argv[]) {
    if (argc == 2)
    {
        Serial serial = Serial(argv[1]);
        serial.createResults();
        serial.printResults();
    }
}
