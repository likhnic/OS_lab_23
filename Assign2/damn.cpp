#include "delep.hpp"

#define BUF_SIZE 20
class delep_command
{
    public:
        delep_command(){};
        ~delep_command(){};
        void execute(){};
        vector<pid_t> pids;
        vector<vector<string> > v;
        vector<string> v1;
        string filepath;
        string response;
        void list_pids(vector<vector<string> > v, vector<pid_t>& pids){
            string fp = v[0][1].c_str();
            DIR* dir = opendir("/proc");
            if (!dir)
            {
                cout<<"There is no such directory"<<endl;
                exit(1);
            }
            for(dirent* ent = readdir(dir);ent!=nullptr;ent=readdir(dir)){
                cout<<ent->d_ino<<" "<<ent->d_name<<" "<<ent->d_off<<" "<<ent->d_reclen<<" "<<ent->d_type<<endl;
                if (!isdigit(ent->d_name[0]))
                    continue;

                pid_t pid = static_cast<pid_t>(stol(ent->d_name));
                stringstream ss;
                ss << "/proc/" << pid << "/fd";
                DIR* fd_dir = opendir(ss.str().c_str());
                if (!fd_dir)
                    continue;
                for(dirent* fd_ent = readdir(fd_dir);fd_ent!=nullptr;fd_ent=readdir(fd_dir)){
                     if (fd_ent->d_name[0] == '.')
                        continue;

                    stringstream link_ss;
                    link_ss << "/proc/" << pid << "/fd/" << fd_ent->d_name;
                    char buf[4096];
                    ssize_t len = readlink(link_ss.str().c_str(), buf, sizeof(buf));
                    if (len != -1){
                        buf[len] = '\0';
                        if (buf == filepath)  pids.push_back(pid);
                    }

                }

                closedir(fd_dir);
            }

            closedir(dir);
        }
        void kill_process(pid_t pid){};
        void remove_file(const string& filepath){};

};

void list_pids(vector<vector<string> > v, vector<pid_t>& pids)
{
    string filepath = v[0][1].c_str();
            DIR* dir = opendir("/proc");
            if (!dir)
            {
                cout<<"There is no such directory"<<endl;
                exit(1);
            }
            for(dirent* ent = readdir(dir);ent!=nullptr;ent=readdir(dir)){
                // cout<<ent->d_ino<<" "<<ent->d_name<<" "<<ent->d_off<<" "<<ent->d_reclen<<" "<<ent->d_type<<endl;
                if (!isdigit(ent->d_name[0]))
                    continue;

                pid_t pid = static_cast<pid_t>(stol(ent->d_name));
                stringstream ss;
                ss << "/proc/" << pid << "/fd";
                DIR* fd_dir = opendir(ss.str().c_str());
                if (!fd_dir)
                    continue;
                for(dirent* fd_ent = readdir(fd_dir);fd_ent!=nullptr;fd_ent=readdir(fd_dir)){
                     if (fd_ent->d_name[0] == '.')
                        continue;

                    stringstream link_ss;
                    link_ss << "/proc/" << pid << "/fd/" << fd_ent->d_name;
                    char buf[4096];
                    ssize_t len = readlink(link_ss.str().c_str(), buf, sizeof(buf));
                    cout<<link_ss.str()<<"\t"<<buf<<endl;
                    if (len != -1){
                        buf[len] = '\0';
                        if (buf == filepath)  pids.push_back(pid);
                    }

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

void get_from_child(int f)
{
    vector<int> pids;
    char buf[BUF_SIZE];
    while (read(f, buf, BUF_SIZE) > 0)
    {
        cout << buf << '\n';
    }
}

int main()
{
    vector<pid_t> pids;
    vector<vector<string> > v;
    vector<string> v1;
    v1.push_back("delep");
    // v1.push_back("/home/venkata/Documents/Sem6/Operating\ Systems/Lab/lab2_/abc.txt");
    v1.push_back("/home/venkata/Documents/Sem6/Operating Systems/Lab/lab2_/abc.txt");
    v.push_back(v1);
    int p[2];
    
    if (pipe(p) < 0)
    {
        perror("error in creating a pipe\n");
        exit(1);
    }

    int pd = fork();
    if (pd < 0)
    {
        perror("error in forking\n");
        exit(1);
    }
    if (pd == 0)
    {
        // start of child
        list_pids(v, pids);
        // end child
        cout << "Processes holding a lock or have the file open:\n";
        for (pid_t pid : pids)
        {
            char buf[20];
            sprintf(buf, "%d", pid);
            write(p[1], buf, 20);
            // cout << pid << '\n';
        }
        exit(0);
    }
    wait(NULL);
    get_from_child(p[0]);
    cout << "Kill these processes and delete the file (y/n)? ";
    string response;
    cin >> response;

    if (response == "y")
    {
        for (pid_t pid : pids)
            kill_process(pid);

        // if (unlink(argv[1]) == -1)
        //     perror("unlink");
    }

    return 0;
}
