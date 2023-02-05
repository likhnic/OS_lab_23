// header file for stringProcessing.cpp

#ifndef STRINGPROCESSING_HPP
#define STRINGPROCESSING_HPP

#include <bits/stdc++.h>
#include <glob.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
using namespace std;

// function to trim a string
string trim(string &s);

// function to split a string into tokens
vector<vector<string> >getAllTokens(string &command);

// function to get all pipes
vector<string> getAllPipes(string &command);

// function to handle * and ?
void handleRegex(vector<string>&cmd, string &temp);

// function to get all tokens
vector<string> getAllCmds(string &segmentCmd);

// function to get all vectored tokens
vector<vector<string> > getAllVectoredTokens(string &command);

// function to run a command
void run_a_command(vector<vector<string> > v,int prev_out,int next_in);

#endif
