#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>

void get_open_pids(const std::string &filename, std::vector<pid_t> &pids) {
    std::string cmd = "lsof -t " + filename;
    FILE *fp = popen(cmd.c_str(), "r");
    if (!fp) {
        std::cerr << "Error: failed to execute lsof command" << std::endl;
        return;
    }

    char buffer[64];
    while (fgets(buffer, sizeof(buffer), fp)) {
        pid_t pid = strtoul(buffer, nullptr, 10);
        pids.push_back(pid);
    }
    pclose(fp);
}

void get_flocked_pids(const std::string &filename, std::vector<pid_t> &pids) {
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error: failed to open file" << std::endl;
        return;
    }

    int lock = flock(fd, LOCK_EX | LOCK_NB);
    if (lock == -1) {
        char buffer[64];
        sprintf(buffer, "fuser -v %s 2>/dev/null", filename.c_str());
        FILE *fp = popen(buffer, "r");
        if (!fp) {
            std::cerr << "Error: failed to execute fuser command" << std::endl;
            return;
        }

        while (fgets(buffer, sizeof(buffer), fp)) {
            char *p = strchr(buffer, ' ');
            if (!p) {
                continue;
            }
            ++p;
            char *q = strchr(p, ' ');
            if (!q) {
                continue;
            }
            *q = '\0';
            pid_t pid = strtoul(p, nullptr, 10);
            pids.push_back(pid);
        }
        pclose(fp);
    }
    close(fd);
}

void kill_processes(const std::vector<pid_t> &pids) {
    for (pid_t pid : pids) {
        std::cout << "Kill process " << pid << "? (y/n) ";
        std::string answer;
std::cin >> answer;
if (answer == "y") {
    kill(pid, SIGKILL);
}
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: delep filename" << std::endl;
        return 1;
    }

    std::string filename(argv[1]);

    std::vector<pid_t> open_pids;
    get_open_pids(filename, open_pids);

    std::vector<pid_t> flocked_pids;
    get_flocked_pids(filename, flocked_pids);

    std::vector<pid_t> pids;
    for (pid_t pid : open_pids) {
        if (std::find(flocked_pids.begin(), flocked_pids.end(), pid) == flocked_pids.end()) {
            pids.push_back(pid);
        }
    }

    if (!pids.empty()) {
        std::cout << "The following processes are holding the file open or locked:" << std::endl;
        for (pid_t pid : pids) {
            std::cout << pid << std::endl;
        }

        kill_processes(pids);
    }

    int status = remove(filename.c_str());
    if (status == -1) {
        std::cerr << "Error: failed to delete file" << std::endl;
        return 1;
    }

    return 0;
}
       
