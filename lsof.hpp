#pragma once

#include <string>
#include <algorithm>

#include "inputParser.hpp"

class LSOF {
private:
    std::string pid;
    std::string command;
    std::string user;
    InputParser inputparser;

    void print_info(const std::string &COMMAND,
                    const std::string &PID,
                    const std::string &USER,
                    const std::string &FD,
                    const std::string &TYPE,
                    const std::string &NODE,
                    const std::string &NAME);
    void readFileInfo(std::string, const std::string &);
    std::string getCOMMAND();
    std::string getUSER();
    void cwdFd();
    void rtdFd();
    void txtFd();
    void memFd();
    void fdFd();

public:
    LSOF() = delete;
    LSOF(const std::string &pid,
         const InputParser &inputparser);
    void run();
};
