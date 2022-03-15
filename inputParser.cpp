#include <algorithm>
#include <unordered_set>
#include "inputParser.hpp"

InputParser::InputParser(int argc, char **argv) {
    Inputtokens.reserve(argc - 1);
    for(int i=1;i<argc;i++) {
        Inputtokens.push_back(argv[i]);
    }
    commandFilter = getCmdOpt("-c");
    typeFilter = getCmdOpt("-t");
    filenameFilter = getCmdOpt("-f");
    pidFilter = getCmdOpt("-p");
}

InputParser::InputParser(const InputParser &obj):
    Inputtokens(obj.Inputtokens),
    commandFilter(obj.commandFilter),
    typeFilter(obj.typeFilter),
    filenameFilter(obj.filenameFilter),
    pidFilter(obj.pidFilter) {}

std::string InputParser::getCmdOpt(const std::string &opt) {
    auto iter = std::find(Inputtokens.begin(), Inputtokens.end(), opt);
    if(iter != Inputtokens.end() && ++iter != Inputtokens.end()) {
        return *iter;
    }
    return "";
}

bool InputParser::isvalid() {
    if(typeFilter.empty()) return true;
    std::unordered_set<std::string> validType {"REG", "CHR", "DIR", "FIFO", "SOCK", "unknown"};
    if(validType.count(typeFilter) == 0) return false;
    return true;
}

std::string InputParser::getCmdFilter() {
    return commandFilter;
}

std::string InputParser::getTypeFilter() {
    return typeFilter;
}

std::string InputParser::getFileNFilter() {
    return filenameFilter;
}

std::string InputParser::getpidFilter() {
    return pidFilter;
}
