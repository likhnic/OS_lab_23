#include "stringProcessing.hpp"

// this is to execute the command
// void run_a_command(vector<vector<string> > v,int prev_out,int next_in);

// executing each command that are piped
void pipe_commands(vector<string> cmds)
{
    int fd[2]; // the file descripter
    int st_in = 0, st_out = 1; // the descriptor for stdin and stdout

    int n = cmds.size();
    for (int i = 0; i < n-1; i++)
    {
        if (pipe(fd) < 0) // creating pipe
        {
            perror("error in creating pipe\n");
            exit(1);
        }

        vector<vector<string>> tokens = getAllVectoredTokens(cmds[i]); // tokenisation of command
        run_a_command(tokens, st_in, fd[1]); // executing the command
        st_in = fd[0]; // updating the input file descriptor
        close(fd[1]); // closing old pipe   
    }

    vector<vector<string>> tokens = getAllVectoredTokens(cmds[n-1]);
    run_a_command(tokens, st_in, st_out);
    if(cmds.back().back()!='&'){
        wait(NULL);
    }
}

void executeCommand(char *command){

    string cmd = command;
    vector<string> toks = getAllPipes(cmd);
    pipe_commands(toks);
}

int main()
{

    // Read the history from a file
    read_history(".history.txt");
    stifle_history(1000);

    while (1)
    {
        // Use readline to get input from the user

        
        string pwd = get_current_dir_name();
        pwd+=" $ ";
        pwd = "bhai shell:"+pwd;
        char *input = readline(pwd.c_str());
        if(strcmp(input, "exit")==0) break;

        // Add the input to the history list
        add_history(input);
        // Write the history to a file
        write_history(".history.txt");

        executeCommand(input);
        // Free the memory used by the input string
        free(input);
    }

    return 0;
}
