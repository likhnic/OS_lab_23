#include <iostream>
#include <fstream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

int main() {
    std::ofstream file("test.txt");
    file.open("test.txt");

    int fd = fileno(file.rdbuf()->_M_file.sys_file());
    flock(fd, LOCK_EX);

    std::cout << "Process " << getpid() << " is holding the file locked" << std::endl;

    while (1) {
        file << "Writing to file" << std::endl;
    }

    return 0;
}
