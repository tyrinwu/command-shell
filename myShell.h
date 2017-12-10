#ifndef __MYSHELL_H__
#define __MYSHELL_H__
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <list>
#include <exception>
#include <assert.h>
#define EXIT_MYSHELL "exit"

// Parsing
void spaceUpdate(std::string & tempStr, std::string::iterator & it, std::vector<std::string> & vecArgs);
void backslashUpdate(std::string & tempStr, std::string::iterator & it, std::string::iterator & itEnd);
std::vector<std::string> parseLine(std::string strInput, std::string d);
void printParsedStr(std::vector<std::string> parsedStr);

// Directory Manipulation
bool isDirCall(std::string command);
void dirCall(std::vector<std::string> newArgs, std::list<std::string> & dirs) ;
int mychdir(std::string newPath);
void pushd(std::string newPath, std::list<std::string> & dirs);
int popd(std::list<std::string> & dirs);
void dirstack(std::list<std::string> & dirs);

// Helper functions of myExecve()
std::vector<std::string> relativeToAbsolute(std::vector<std::string> parsedArgs);
bool hasSlash(std::string filePath) ;
std::string getDir(std::string cmdWithPath);
std::string getFileName(std::string cmdWithPath);
std::vector<std::string> getPaths();
std::vector<std::string> cmdsFromGetPaths(std::string cmd, std::vector<std::string> paths);
char ** vecToCharStarStar(std::vector<std::string> parsedArgs); 
void free_vecToCharStarStar(char ** cString) ; // Free the memory allocated by vecToCharStarStar

// Shell
bool isEnd(std::vector<std::string> parsedArgs);
void printShell(void);
int myExecve(std::vector<std::string> parsedArgs, char ** envp);
int runCommand(std::vector<std::string> parsedArgs);
int runShell(std::istream & stream);  

#endif
