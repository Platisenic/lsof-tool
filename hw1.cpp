#include <iostream>
#include <string>
#include <vector>

#include "lsof.hpp"
#include "inputParser.hpp"
#include "utils.hpp"


int main(int argc, char **argv) {
    InputParser inputparser(argc, argv);
    if(!inputparser.isvalid()) {
        std::cerr << "Invalid TYPE option" << std::endl;
        return 1;
    }

    std::vector<int> pids = getpids();

    print_header();
    for(int pid: pids) {
        LSOF lsof(std::to_string(pid), inputparser);
        lsof.run();
    }

    return 0;
}
