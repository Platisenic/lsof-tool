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

#include "lsof.hpp"
#include "utils.hpp"

void LSOF::print_info(const std::string &COMMAND,
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

    std::cout << std::left << std::setfill(' ')
              << std::setw(30) << COMMAND
              << std::setw(8)  << PID
              << std::setw(10) << USER
              << std::setw(5)  << FD
              << std::setw(10)  << TYPE
              << std::setw(10) << NODE
              << std::setw(30) << NAME
              << std::endl;
}

LSOF::LSOF(const std::string &pid,
           const InputParser &inputparser):
    pid(pid),
    command(getCOMMAND()),
    user(getUSER()),
    inputparser(inputparser) {}

std::string LSOF::getCOMMAND() {
    std::ifstream infile("/proc/" + pid + "/comm");
    std::string cmd;
    if (!(infile >> cmd)) return "";
    return cmd;
}

std::string LSOF::getUSER() {
    struct stat sb;
    struct passwd *pw;
    if (stat(("/proc/" + pid).c_str(), &sb) == -1) return "";
    pw = getpwuid(sb.st_uid);
    return std::string(pw->pw_name);
}

void LSOF::run() {
    if (command.empty() || user.empty()) return;
    cwdFd();
    rtdFd();
    txtFd();
    memFd();
    fdFd();
}

void LSOF::cwdFd() {
    readFileInfo("/proc/" + pid + "/cwd", "cwd");
}

void LSOF::rtdFd() {
    readFileInfo("/proc/" + pid + "/root", "rtd");
}

void LSOF::txtFd() {
    readFileInfo("/proc/" + pid + "/exe", "txt");
}

void LSOF::memFd() {
    std::ifstream infile("/proc/" + pid + "/maps");
    std::string line;
    std::string t, inode, filename, deleted;
    std::string previnode = "";
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
}

void LSOF::fdFd() {
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(("/proc/" + pid + "/fd").c_str())) == NULL) {
        print_info(command, pid, user, "NOFD", "", "", "/proc/" + pid + "/fd (Permission denied)");
        return;
    }
    while ((dirp = readdir(dp)) != NULL) {
        if (isnumber(std::string(dirp->d_name))) {
            std::ifstream infile("/proc/" + pid + "/fdinfo/" + std::string(dirp->d_name));
            std::string t, FD;
            int flags;
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
}

void LSOF::readFileInfo(std::string filename, const std::string &FD) {
    struct stat sb;
    char realFileName[PATH_MAX] = {0};
    std::string TYPE = "unknown";
    std::string NODE = "";
    
    if (stat(filename.c_str(), &sb) == -1) {
        if(errno == EACCES) {
            filename += " (Permission denied)";
            print_info(command, pid, user, FD, TYPE, NODE, filename);
        }
        return;
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

    if (lstat(filename.c_str(), &sb) == -1) return;
    if((sb.st_mode & S_IFMT) == S_IFLNK) {
        if (readlink(filename.c_str(), realFileName, PATH_MAX) == -1) return;
        filename = realFileName;
    }

    print_info(command, pid, user, FD, TYPE, NODE, filename);
    return;
}
