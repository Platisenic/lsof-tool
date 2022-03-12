#include <iomanip>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>

#include "lsof.hpp"

int main() {
    DIR *dp;
    struct dirent *dirp;

    print_info("COMMAND", "PID", "USER", "FD", "TYPE", "NODE", "NAME");

    if ((dp = opendir("/proc")) == NULL) {
        perror("cannot open proc");
    }
    int i = 0;
    while ((dirp = readdir(dp)) != NULL) {
        if(i==20) break;
        std::string pid(dirp->d_name);
        if (is_number(pid)) {
            LSOF lsof(pid);
            lsof.run();
            i++;
        }
    }
    closedir(dp);
    return 0;
}
