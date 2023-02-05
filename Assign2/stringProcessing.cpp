#include "stringProcessing.hpp"

string trim(string &s){
    string temp = "";
    int i=0;
    int j=s.size()-1;
    for(i=0;i<s.length();++i){
        if(s[i]!=' ')break;
    }
    for(j=s.length()-1;j>=0;--j){
        if(s[j]!=' ')break;
    }
    return s.substr(i,j-i+1);
}

vector<vector<string> > getAllTokens(string &command){

    vector<vector<string> > allPipes;
    vector<string> pipe;
    string temp = "";
    int doubleQuotes = 0;
    int singleQuotes = 0;

    for(int i = 0; i < command.length();++i){
        if(command[i]=='"'){
            doubleQuotes = !doubleQuotes;
            temp+=command[i];
        }
        else if(command[i]=='\''){
            singleQuotes = !singleQuotes;
            temp+=command[i];
        }
        else if(command[i]=='\\'){
            temp+=command[i+1];
            i++;
            continue;
        }
        else if(command[i]==' ' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")pipe.push_back(trim(temp));
            temp = "";
        }
        else if(command[i]=='>' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")pipe.push_back(trim(temp));
            pipe.push_back(">");
            temp = "";
        }
        else if(command[i]=='<' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")pipe.push_back(trim(temp));
            pipe.push_back("<");
            temp = "";
        }
        else if(command[i]=='&' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")pipe.push_back(trim(temp));
            pipe.push_back("&");
            temp = "";
        }
        else if(command[i]=='|' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")pipe.push_back(trim(temp));
            temp = "";
            if(pipe.size())allPipes.push_back(pipe);
            pipe.clear();
        }
        else temp+=command[i];
    }

    if(trim(temp)!=""){
        pipe.push_back(trim(temp));
        allPipes.push_back(pipe);
    }

    // for(auto x:allPipes){
    //     for(auto y:x){
    //         cout<<y<<" gap ";
    //     }
    //     cout<<'\n';
    // }

    return allPipes;
}

vector<string> getAllPipes(string &command){

    vector<string> pipes;
    string temp = "";

    int doubleQuotes = 0;
    int singleQuotes = 0;

    for(int i = 0; i < command.length();++i){
        if(command[i]=='"'){
            doubleQuotes = !doubleQuotes;
            temp+=command[i];
        }
        else if(command[i]=='\''){
            singleQuotes = !singleQuotes;
            temp+=command[i];
        }
        else if(command[i]=='\\'){
            temp+=command[i+1];
            i++;
            continue;
        }
        else if(command[i]=='|' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")pipes.push_back(trim(temp));
            temp = "";
        }
        else temp+=command[i];
    }

    if(trim(temp)!=""){
        pipes.push_back(trim(temp));
    }

    // for(auto x:pipes){
    //     cout<<x<<" gap ";
    // }
    // cout<<"\n";

    return pipes;
}

vector<string> getAllCmds(string &segmentCmd){

    int doubleQuotes = 0;
    int singleQuotes = 0;

    vector<string> cmds;
    string temp="";
    for(int i=0;i<segmentCmd.length();++i){
        if(segmentCmd[i]=='"'){
            doubleQuotes = !doubleQuotes;
            temp+=segmentCmd[i];
        }
        else if(segmentCmd[i]=='\''){
            singleQuotes = !singleQuotes;
            temp+=segmentCmd[i];
        }
        else if(segmentCmd[i]=='\\'){
            i++;
            temp+=segmentCmd[i];
            continue;
        }
        else if(segmentCmd[i]=='&' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")cmds.push_back(trim(temp));
            cmds.push_back("&");
            temp="";
        }
        else if(segmentCmd[i]=='<' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")cmds.push_back(trim(temp));
            cmds.push_back("<");
            temp="";
        }
        else if(segmentCmd[i]=='>' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")cmds.push_back(trim(temp));
            cmds.push_back(">");
            temp="";
        }
        else if(segmentCmd[i]==' ' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!="")cmds.push_back(trim(temp));
            temp="";
        }
        else temp+=segmentCmd[i];
    }

    if(trim(temp)!="")cmds.push_back(trim(temp));

    // for(auto x:cmds){
    //     cout<<x<<" gap ";
    // }
    // cout<<"\n";

    return cmds;
}


void handleRegex(vector<string>&cmd, string &temp){

    glob_t glob_result;
    int glob_ret;
    glob_ret = glob(temp.c_str(), 0, NULL, &glob_result);

    int n = glob_result.gl_pathc;
    for (int i = 0; i < n; i++)
    {
        cmd.push_back(glob_result.gl_pathv[i]);
    }
    temp = "";
}

vector<vector<string> >getAllVectoredTokens(string &commands){

    vector<vector<string> > allCmds;
    vector<string> cmds;
    string temp = "";
    int doubleQuotes = 0;
    int singleQuotes = 0;
    int tokenContainsReg = 0;
    
    for(int i = 0; i < commands.length();++i){
        if(commands[i]=='"'){
            doubleQuotes = !doubleQuotes;
            temp+=commands[i];
        }
        else if(commands[i]=='\''){
            singleQuotes = !singleQuotes;
            temp+=commands[i];
        }
        else if(commands[i]=='\\'){
            if(commands[i+1]=='*' || commands[i+1]=='?'){
                tokenContainsReg = 1;
            }
            temp+=commands[i+1];
            i++;
            continue;
        }
        else if(commands[i]==' ' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!=""){
                if(tokenContainsReg)handleRegex(cmds, temp);
                else cmds.push_back(temp);
                tokenContainsReg = 0;
            }
            temp="";
        }
        else if(commands[i]=='>' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!=""){
                if(tokenContainsReg)handleRegex(cmds, temp);
                else cmds.push_back(temp);
                tokenContainsReg = 0;
            }
            if(cmds.size())allCmds.push_back(cmds);
            cmds.clear();
            allCmds.push_back({">"});
            temp="";
        }
        else if(commands[i]=='<' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!=""){
                if(tokenContainsReg)handleRegex(cmds, temp);
                else cmds.push_back(temp);
                tokenContainsReg = 0;
            }
            if(cmds.size())allCmds.push_back(cmds);
            cmds.clear();
            allCmds.push_back({"<"});
            temp="";
        }
        else if(commands[i]=='&' && !doubleQuotes && !singleQuotes){
            if(trim(temp)!=""){
                if(tokenContainsReg)handleRegex(cmds, temp);
                else cmds.push_back(temp);
                tokenContainsReg = 0;
            }
            if(cmds.size())allCmds.push_back(cmds);
            cmds.clear();
            allCmds.push_back({"&"});
            temp="";
        }
        else{
            temp+=commands[i];
            if(commands[i]=='*' || commands[i]=='?'){
                tokenContainsReg = 1;
            }
        }
    }

    if(trim(temp)!=""){
        if(tokenContainsReg)handleRegex(cmds, temp);
        else cmds.push_back(temp);
        tokenContainsReg = 0;
        allCmds.push_back(cmds);
    }

    // for(auto x:allCmds){
    //     for(auto y:x){
    //         cout<<y<<" ";
    //     }
    //     cout<<'\n';
    // }

    return allCmds;
}

int check_flags(int read_flag,int write_flag,int background_flag){
    return !(read_flag|write_flag|background_flag); 
}

void run_a_command(vector<vector<string> > v,int prev_out,int next_in){
    int write_to_file_flag=0,read_from_file_flag=0,background_process_flag=0,n=0;
    string write_to_file,read_from_file;
    for(int i = 0;i<v.size();++i){
        if(i<v.size()-1){
            if(v[i][0].compare(">")==0){
                write_to_file = v[i+1][0];
                write_to_file_flag = 1;
                cout<<write_to_file<<endl;
            }
            if(v[i][0].compare("<")==0){
                read_from_file = v[i+1][0];
                read_from_file_flag = 1;
                cout<<read_from_file<<endl;
            }
        }
        if(v[i][0].compare("&")==0){
            background_process_flag = 1;
        }
        if(check_flags(read_from_file_flag,write_to_file_flag,background_process_flag)){
                n+=v[i].size();
        }
    }
    char* command[n+1];
    int j  = 0,l=0;
    for(int i = 0;i<v.size()&&j<=n;++i){
        if(j==n){
            command[j++] = NULL;
        }else{
            for(int k = 0;k<v[i].size();++k){
                command[j++] = const_cast<char*>(v[i][k].c_str());
                // cout<<command[j-1]<<" ";
            }
        }
    }
    int pid = fork();
    if(pid==0){
        // writing 
        if(write_to_file_flag){
            next_in = open(write_to_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(next_in,1);
        }
        //reading
        if(read_from_file_flag){
            prev_out = open(read_from_file.c_str(),O_RDONLY,0644);
            dup2(prev_out,0);
        }
        execvp(command[0],command);
        exit(1); 
    }else{
        if(background_process_flag){
            cout<<"Background process"<<endl;
        }else{
            wait(NULL);
            printf("Waited and child is done\n");
        }
    }


}



// int main(){

//     string command = "cd \"Assign\\ 2\" \'|\' ls -l | grep \"a.out\" > output.txt";
//     getAllPipes(command);

//     string segmentCmd = "ls -la < cmd1 > cmd2 /home/likhith26090/Documents/projects/*.c";

//     getAllCmds(segmentCmd);

//     getAllVectoredTokens(segmentCmd);

//     return 0;
// }
