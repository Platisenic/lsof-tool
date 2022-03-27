#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "lsof.hpp"
#include "utils.hpp"

void LSOF::save_info(const std::string &COMMAND,
                const std::string &PID,
                const std::string &USER,
                const std::string &FD,
                const std::string &TYPE,
                const std::string &NODE,
                const std::string &NAME) {

    if (!inputparser.getCmdFilter().empty() &&
        !std::regex_search(COMMAND, std::regex(inputparser.getCmdFilter()))) return;
    
    if (!inputparser.getpidFilter().empty() &&
        !std::regex_search(PID, std::regex(inputparser.getpidFilter()))) return;
    
    if (!inputparser.getTypeFilter().empty() &&
        !std::regex_search(TYPE, std::regex(inputparser.getTypeFilter()))) return;
    
    if (!inputparser.getFileNFilter().empty() &&
        !std::regex_search(NAME, std::regex(inputparser.getFileNFilter()))) return;

    printBuffer << std::left << std::setfill(' ')
                << std::setw(30) << COMMAND
                << std::setw(8)  << PID
                << std::setw(10) << USER
                << std::setw(5)  << FD
                << std::setw(10)  << TYPE
                << std::setw(10) << NODE
                << std::setw(30) << NAME
                << std::endl;
}

void LSOF::print_info() {
    std::cout << printBuffer.str();
}

LSOF::LSOF(const std::string &pid,
           const InputParser &inputparser):
    pid(pid),
    command(getCOMMAND()),
    user(getUSER()),
    inputparser(inputparser),
    printBuffer(""),
    previnode("") {}

std::string LSOF::getCOMMAND() {
    std::ifstream infile("/proc/" + pid + "/comm");
    std::string cmd;
    if(!infile.is_open()) return "";
    if (!(infile >> cmd)) return "";
    return cmd;
}

std::string LSOF::getUSER() {
    struct stat sb;
    struct passwd *pw;
    if (stat(("/proc/" + pid).c_str(), &sb) == -1) return "";
    if ((pw = getpwuid(sb.st_uid)) == NULL) return "";
    return std::string(pw->pw_name);
}

void LSOF::run() {
    if (command.empty() || user.empty()) return;
    if(!cwdFd()) return;
    if(!rtdFd()) return;
    if(!txtFd()) return;
    if(!memFd()) return;
    if(!fdFd()) return;
    print_info();
}

bool LSOF::cwdFd() {
    return readFileInfo("/proc/" + pid + "/cwd", "cwd");
}

bool LSOF::rtdFd() {
    return readFileInfo("/proc/" + pid + "/root", "rtd");
}

bool LSOF::txtFd() {
    return readFileInfo("/proc/" + pid + "/exe", "txt");
}

bool LSOF::memFd() {
    std::ifstream infile("/proc/" + pid + "/maps");
    std::string line;
    std::string t, inode, filename, deleted;

    if(!infile.is_open()) {
        if(errno == EACCES) return true;
        return false;
    }

    while(std::getline(infile, line)) {
        std::istringstream iss(line);
        if (!(iss >> t >> t >> t >> t >> inode)) continue;
        if(inode != "0" && inode != previnode) {
            if(!(iss >> filename)) continue;
            if(iss >> deleted && deleted == "(deleted)") {
                readFileInfo(filename, "DEL");
            }else {
                readFileInfo(filename, "mem");
            }
            previnode = inode;
        }
    }
    return true;
}

bool LSOF::fdFd() {
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(("/proc/" + pid + "/fd").c_str())) == NULL) {
        save_info(command, pid, user, "NOFD", "", "", "/proc/" + pid + "/fd (Permission denied)");
        return true;
    }
    while ((dirp = readdir(dp)) != NULL) {
        if (isnumber(std::string(dirp->d_name))) {
            std::ifstream infile("/proc/" + pid + "/fdinfo/" + std::string(dirp->d_name));
            std::string t, FD;
            int flags;
            if(!infile.is_open()) continue;
            infile >> t >> t >> t >> std::oct >> flags;
            FD = dirp->d_name;
            switch(flags & O_ACCMODE) {
                case O_RDONLY: FD += "r"; break;
                case O_WRONLY: FD += "w"; break;
                case O_RDWR: FD += "u"; break;
                default: break;
            }
            readFileInfo("/proc/" + pid + "/fd/" + std::string(dirp->d_name), FD);
        }
    }
    closedir(dp);
    return true;
}

bool LSOF::readFileInfo(std::string filename, const std::string &FD) {
    struct stat sb;
    char realFileName[PATH_MAX] = {0};
    std::string TYPE = "unknown";
    std::string NODE = "";
    
    if (stat(filename.c_str(), &sb) == -1) {
        if(errno == EACCES) {
            filename += " (Permission denied)";
            save_info(command, pid, user, FD, TYPE, NODE, filename);
            return true;
        }
        return false;
    }

    NODE = std::to_string(sb.st_ino);

    switch (sb.st_mode & S_IFMT) {
        case S_IFDIR:  TYPE = "DIR";       break;
        case S_IFREG:  TYPE = "REG";       break;
        case S_IFCHR:  TYPE = "CHR";       break;
        case S_IFIFO:  TYPE = "FIFO";      break;
        case S_IFSOCK: TYPE = "SOCK";      break;
        default:       TYPE = "unknown";   break;
    }

    if (lstat(filename.c_str(), &sb) == -1) return false;
    if((sb.st_mode & S_IFMT) == S_IFLNK) {
        if (readlink(filename.c_str(), realFileName, PATH_MAX) == -1) return false;
        filename = realFileName;
    }
    previnode = NODE;
    save_info(command, pid, user, FD, TYPE, NODE, filename);
    return true;
}
