#pragma once

#include <string>
#include <algorithm>
#include <sstream>
#include "inputParser.hpp"

class LSOF {
private:
    std::string pid;
    std::string command;
    std::string user;
    InputParser inputparser;
    std::stringstream printBuffer;
    std::string previnode = "";

    void save_info(const std::string &COMMAND,
                    const std::string &PID,
                    const std::string &USER,
                    const std::string &FD,
                    const std::string &TYPE,
                    const std::string &NODE,
                    const std::string &NAME);
    void print_info();
    bool readFileInfo(std::string, const std::string &);
    std::string getCOMMAND();
    std::string getUSER();
    bool cwdFd();
    bool rtdFd();
    bool txtFd();
    bool memFd();
    bool fdFd();

public:
    LSOF() = delete;
    LSOF(const std::string &pid,
         const InputParser &inputparser);
    void run();
};
