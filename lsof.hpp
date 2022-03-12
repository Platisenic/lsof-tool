#pragma once

#include <string>
#include <algorithm>

bool is_number(std::string s);

void print_info(std::string COMMAND,
                std::string PID,
                std::string USER,
                std::string FD,
                std::string TYPE,
                std::string NODE,
                std::string NAME);

class LSOF {
private:
    std::string pid;
    std::string command;
    std::string user;

    void readFileInfo(std::string, std::string);
    std::string getCOMMAND();
    std::string getUSER();
    void parsearg();
    void cwdFd();
    void rtdFd();
    void txtFd();
    void memFd();
    void fdFd();
    void noFd();

public:
    LSOF() = delete;
    LSOF(std::string pid);
    void run();
};