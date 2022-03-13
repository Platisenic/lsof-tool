#pragma once

#include <vector>
#include <string>

class InputParser{
private:
    std::vector <std::string> Inputtokens;
    std::string commandFilter;
    std::string typeFilter;
    std::string filenameFilter;
    std::string pidFilter;

    std::string getCmdOpt(const std::string &opt);
public:
    InputParser() = delete;
    InputParser(int argc, char **argv);
    InputParser(const InputParser &obj);
    bool isvalid();
    std::string getCmdFilter();
    std::string getTypeFilter();
    std::string getFileNFilter();
    std::string getpidFilter();
};
