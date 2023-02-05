#include "stringProcessing.hpp"

// this is to execute the command
void execute_command(vector<vector<string>> command, int in, int out);

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
        execute_command(tokens, st_in, fd[1]); // executing the command
        st_in = fd[0]; // updating the input file descriptor
        close(fd[1]); // closing old pipe
    }

    vector<vector<string>> tokens = getAllVectoredTokens(cmds[n-1]);
    execute_command(tokens, st_in, st_out);
}
