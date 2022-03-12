#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>


#include "lsof.hpp"

bool is_number(std::string s) {
    return !s.empty() && std::find_if(s.begin(), s.end(),
        [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void print_info(std::string COMMAND,
                std::string PID,
                std::string USER,
                std::string FD,
                std::string TYPE,
                std::string NODE,
                std::string NAME) {

    std::cout << std::left
              << std::setw(30) << COMMAND
              << std::setw(5)  << " " << PID
              << std::setw(10) << " " << USER
              << std::setw(5)  << " " << FD
              << std::setw(8)  << " " << TYPE
              << std::setw(10) << " " << NODE
              << std::setw(30) << " " << NAME
              << std::endl;
}

LSOF::LSOF(std::string pid):
    pid(pid),
    command(getCOMMAND()),
    user(getUSER()) {}

std::string LSOF::getCOMMAND() {
    std::ifstream infile("/proc/" + pid + "/comm");
    std::string cmd;
    if(!(infile >> cmd)) {
        // std::cerr << "iss error";
        return "";
    }
    return cmd;
}

std::string LSOF::getUSER() {
    struct stat sb;
    struct passwd *pw;
    if (stat(("/proc/" + pid).c_str(), &sb) == -1) {
        perror("stat error");
    }
    pw = getpwuid(sb.st_uid);
    return std::string(pw->pw_name);
}

void LSOF::run() {
    cwdFd();
    rtdFd();
    txtFd();
    memFd();
    fdFd();
    noFd();
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
        if (!(iss >> t >> t >> t >> t >> inode)) {
            std::cerr << "iss error"; 
        }
        if(inode != "0" && inode != previnode) {
            if(!(iss >> filename)) {
                std::cerr << "iss error";
            }
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
        // perror("cannot open procfd");
        return;
    }
    while ((dirp = readdir(dp)) != NULL) {
        if (is_number(std::string(dirp->d_name))) {
            std::ifstream infile("/proc/" + pid + "/fdinfo/" + std::string(dirp->d_name));
            std::string t, FD;
            int flags;
            infile >> t >> t >> t >> std::oct >> flags;
            FD = dirp->d_name;
            switch(flags & O_ACCMODE) {
                case O_RDONLY: FD += "r"; break;
                case O_WRONLY: FD += "w"; break;
                case O_RDWR: FD += "u"; break;
                default: std::cerr << "file mode error\n"; break;
            }
            readFileInfo("/proc/" + pid + "/fd/" + std::string(dirp->d_name), FD);
        }
    }
    closedir(dp);
}

void LSOF::noFd() {
    readFileInfo("/proc/" + pid + "/fd", "NOFD");
}

void LSOF::readFileInfo(std::string filename, std::string FD) {
    struct stat sb;
    char realFileName[PATH_MAX] = {0};
    std::string TYPE = "unknown";
    std::string NODE = "";
    if (lstat(filename.c_str(), &sb) == -1) {
        perror("lstat error");
    }
    
    if((sb.st_mode & S_IFMT) == S_IFLNK) {
        if (readlink(filename.c_str(), realFileName, PATH_MAX) == -1) {
            if(errno == EACCES) {
                filename += " (Permission denied)";
            }
            print_info(command, pid, user, FD, TYPE, NODE, filename);
            return;
        }
        filename = realFileName;
    }

    if (stat(filename.c_str(), &sb) == -1) {
        if(errno == EACCES) {
            filename += " (Permission denied)";
        }
        print_info(command, pid, user, FD, TYPE, NODE, filename);
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
    print_info(command, pid, user, FD, TYPE, NODE, filename);
    return;
}


