#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

#include <sys/types.h>
#include <dirent.h>


bool isnumber(std::string s) {
    return !s.empty() && std::find_if(s.begin(), s.end(),
        [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void print_header() {

    std::cout << std::left
              << std::setw(30) << "COMMAND"
              << std::setw(5)  << " " << "PID"
              << std::setw(10) << " " << "USER"
              << std::setw(5)  << " " << "FD"
              << std::setw(8)  << " " << "TYPE"
              << std::setw(10) << " " << "NODE"
              << std::setw(30) << " " << "NAME"
              << std::endl;
}

std::vector<int> getpids() {
    DIR *dp;
    struct dirent *dirp;
    std::vector<int> pids;

    if ((dp = opendir("/proc")) == NULL) {
        perror("cannot open proc");
    }
    while ((dirp = readdir(dp)) != NULL) {
        std::string pid(dirp->d_name);
        if (isnumber(pid)) {
            pids.push_back(std::stoi(pid));
        }
    }
    closedir(dp);

    sort(pids.begin(), pids.end());

    return pids;
}

