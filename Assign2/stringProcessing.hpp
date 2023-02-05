// header file for stringProcessing.cpp

#ifndef STRINGPROCESSING_HPP
#define STRINGPROCESSING_HPP

#include <bits/stdc++.h>
#include <glob.h>
#include<unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
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


#endif
