#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
using namespace std;
void list_pids(const string& filepath, vector<pid_t>& pids)
{
    DIR* dir = opendir("/proc");
    if (!dir)
    {
        perror("opendir");
        return;
    }

    dirent* ent;
    while ((ent = readdir(dir)) != nullptr)
    {
        if (!isdigit(ent->d_name[0]))
            continue;

        pid_t pid = static_cast<pid_t>(stol(ent->d_name));
        stringstream ss;
        ss << "/proc/" << pid << "/fd";
        DIR* fd_dir = opendir(ss.str().c_str());
        if (!fd_dir)
            continue;

        dirent* fd_ent;
        while ((fd_ent = readdir(fd_dir)) != nullptr)
        {
            if (fd_ent->d_name[0] == '.')
                continue;

            stringstream link_ss;
            link_ss << "/proc/" << pid << "/fd/" << fd_ent->d_name;
            char buf[4096];
            ssize_t len = readlink(link_ss.str().c_str(), buf, sizeof(buf));
            if (len == -1)
                continue;

            buf[len] = '\0';
            if (buf == filepath)
                pids.push_back(pid);
        }

        closedir(fd_dir);
    }

    closedir(dir);
}

void kill_process(pid_t pid)
{
    if (kill(pid, SIGKILL) == -1)
        perror("kill");
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filepath>\n";
        return 1;
    }

    vector<pid_t> pids;
    list_pids(argv[1], pids);

    cout << "Processes holding a lock or have the file open:\n";
    for (pid_t pid : pids)
        cout << pid << '\n';

    // cout << "Kill these processes and delete the file (yes/no)? ";
    // string response;
    // cin >> response;

    // if (response == "yes")
    // {
    //     for (pid_t pid : pids)
    //         kill_process(pid);

    //     if (unlink(argv[1]) == -1)
    //         perror("unlink");
    // }

    return 0;
}
