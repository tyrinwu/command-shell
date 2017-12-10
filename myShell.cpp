#include "myShell.h"

/* Parsing */
void printParsedStr(std::vector<std::string> parsedStr) {
    std::vector<std::string>::iterator itParsed = parsedStr.begin();
    std::cout << "<";
    while (itParsed != parsedStr.end()) {
        if (itParsed != parsedStr.begin()){
            std::cout << ",";
        }
        std::cout << *itParsed;
        ++itParsed;
    }
    std::cout << ">" <<std::endl;
}

void backslashUpdate(std::string & tempStr,
                     std::string::iterator & it,
                     std::string::iterator & itEnd){
    /* -Functionality: This is a subroutine in parseLine() function. It
     *      updates tempStr while meeting "\" in the while loop.
     * -Corner cases:
     *      "\" being the last character as in "abc\".
     *      Treatment: I ignore the case when "\" happens to be the last
     *      character in the string to be parsed by adding the if
     *      statement.
     *
     *      Also, note that even though some escape is not
     *      valid in bash shell, for example, "\a", I will pretend we can
     *      escape it and add "a" to tempStr.
     **/
    ++it;
    if (it != itEnd) {
      tempStr += *it;
    }
}
void spaceUpdate(std::string & tempStr,
                 std::string::iterator & it,
                 std::vector<std::string> & vecArgs) {
    /* -Function: This is another subroutine in parseLine()
     *      function. While meeting a whitespace, the delimiter, it
     *      checks whether tempStr is empty. If not empty, it pushes
     *      tempStr to the vector of arguments.
     * -Corner cases:
     *      The corner cases actually lies in the function, parseLine(). spaceUpdate
     *      would "not" be called when last char in the orginal string to parse is not
     *      whitespace, the trigger. Please see the parseLine() function for more i
     *      information.
     * */
    if (tempStr != "") {
        vecArgs.push_back(tempStr);
        tempStr = "";
    }
}
std::vector<std::string> parseLine(std::string strInput, std::string delimiter){
    /* -Function: The function takes a string and parse it using the
     *      delimiter users decides while paying attention to the
     *      escaper character \.
     *
     * Input : [std::string] strInput
     * Subroutines: spaceUpdate(), backslashUpdate()
     * Output: [std::vector<std::string>] vecArgs
     * */
    std::vector<std::string> vecArgs;
    std::string::iterator it = strInput.begin();
    std::string::iterator itEnd = strInput.end();
    std::string tempStr;
    std::string de = delimiter;
    std::string BACKSLASH = "\\";

    while (it != strInput.end()) {
        std::string thisChar (1, *it);
        if (thisChar == de) {
            spaceUpdate(tempStr, it, vecArgs);
        } else if (thisChar == BACKSLASH) {
            backslashUpdate(tempStr, it, itEnd);
        } else {
            tempStr = tempStr + thisChar;
        }
        ++it;
    }
    // Need to update again since the last character might not be
    // space and leave the last tempStr not added into vecArgs
    spaceUpdate(tempStr, it, vecArgs);
    return vecArgs;
}
char ** vecToCharStarStar(std::vector<std::string> parsedArgs){
    /* -Function: This function changes a std::vector<std::string> into char **
     *    so that execve function can take it as arguments.(Note that we
     *    also need to add a NULL pointer for execve function. Also,
     *    NULL pointer is useful to detect whether we reach the
     *    end of array.)*/
    std::vector<std::string>::iterator it = parsedArgs.begin();
    char **  cStrings = new char*[parsedArgs.size()+1];
    int idx = 0;
    while ( it != parsedArgs.end() ) {
        char * strToAdd = strdup(parsedArgs[idx].c_str());
        cStrings[idx] =strToAdd;
        ++it;
        ++idx;
    }
    cStrings[parsedArgs.size()] = NULL;
    return cStrings;
}
void free_vecToCharStarStar (char ** cString) {
    /* free the char ** created by vecToCharStarStar function. */
    char ** ptr = cString;
    while (*ptr != NULL) {
        free(*ptr);
        ++ptr;
    }
    delete[] cString;
}
/* END of Parsing*/

/* Directory Manipulation*/
bool isDirCall(std::string command){
   if ( (command == "cd") || (command == "pushd") || (command == "popd") || (command == "dirstack") ) {
     return true;
   } else {
     return false;
   }
}
int mychdir(std::string newPath) {
    int result = chdir(newPath.c_str());
    if (result == -1){
        perror("Fail to change directory.");
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}
void pushd(std::string newPath, std::list<std::string> & dirs) {
    /* Function: this function first push its current directory into the
     *  directory stack that we maintain and try to change directory.
     *
     * Note: If the change of directory fails, it should pop the directory pushed
     *  previously and return.
     *
     **/
    //TODO add test case here
    char * dirName = get_current_dir_name();
    std::string currPath = dirName;
    dirs.push_back(currPath);
    if (mychdir(newPath) == EXIT_FAILURE) {
        free(dirName);
        dirs.pop_back();
        return;
    }
    free(dirName);
}
int popd(std::list<std::string> & dirs) {
    /* Function: This function must has one invariant: it cannot be called 
     *  while the size of the list is 0.
     * */
    if (dirs.size() == 0) {
        perror("The directory stack is empty.");
        return EXIT_FAILURE;
    }
    std::string newPath = dirs.back();
    dirs.pop_back();
    if (mychdir(newPath) == EXIT_FAILURE) {
        std::cout << "Fail to change directory while pushing.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void dirstack(std::list<std::string> & dirs) {
    /*-Function: utilize iterator to print underlying data of each node
     *  in the linkedlist.*/
    std::list<std::string>::iterator it = dirs.begin();
    while (it != dirs.end()) {
        std::cout << *it << std::endl;
        ++it;
    }
}
void dirCall(std::vector<std::string> newArgs, std::list<std::string> & dirs) {
   /*-Function: It first checks whether the number of input is correct for each
    *   function. For example, pushd() needs "pushd Something", thus the
    *   length is 2. popd() on the other hand, don't need extra input.
    *   Then, perform the function indicated by command.
    * */
   std::string cmd = newArgs[0];
   int argc = newArgs.size();
   if (cmd == "pushd") {
       if (argc != 2) {
           perror("Wrong number of parameters for pushd.");
       } else {
           pushd(newArgs[1], dirs);
       }
   } else if (cmd == "cd"){
   	if (argc != 2) {
           perror("Wrong number of parameters for cd.");
	} else {
	   mychdir(newArgs[1]);
	}
   } else if (cmd == "popd") {
       if (argc != 1) {
           perror("Wrong number of parameters for popd.");
       } else {
           popd(dirs);
       }
   } else if (cmd == "dirstack") {
       if (argc != 1) {
           perror("Wrong number of parameters for dirstack.");
       } else {
           dirstack(dirs);
       }
   } else {
       perror("Invalid dirCall.");
   }
}
/* END of Directory Manipulation*/

/* Helper functions of myExecve */
bool hasSlash(std::string filePath) {
    /* -Function: this function utilize rfind in the c++ string class.
     *    If it cannot find the key, "/", it would return
     *    std::string::npos, which is -1, a constant.
     * */
    size_t pos = filePath.rfind("/");
    if (pos != std::string::npos) {
        return true;
    }
    return false;
}
std::vector<std::string> relativeToAbsolute(std::vector<std::string> parsedArgs) {
    /* -Function: change the all the relative paths to absolute paths.
     *      Note we only need to check whether the first two characters
     *      of a string is "./" or ".."*/
    char * cwd = get_current_dir_name();
    std::string cwdStr = cwd;
    std::string DOT = ".";
    std::string SLASH = "/";
    for (size_t i = 0 ; i < parsedArgs.size(); ++i) {
        /* If no slash or of size 1, there is no need to check whether
         * it is a path.*/
        if (hasSlash(parsedArgs[i]) && (parsedArgs[i].size() > 1) ) {
            std::string s1 (1, parsedArgs[i].at(0) );
            std::string s2 (1, parsedArgs[i].at(1) );
            /*  Case 1: Begins "./" */
            if ((DOT == s1) && (SLASH == s2)) {
                std::string newPath = cwdStr + parsedArgs[i].substr(1, std::string::npos);
                parsedArgs[i] = newPath;
            /*  Case 2: Begins ".." */
            } else if ((DOT == s1) && (DOT == s2)) {
                std::string newPath = cwdStr + "/" + parsedArgs[i].substr(0, std::string::npos);
                parsedArgs[i] = newPath;
            }
        }
    }
    free(cwd);
    return parsedArgs;
}
std::string getDir(std::string cmdWithPath){
    /* Funtion: Utilize rfind and substr function found in cplusplus.com to
     *    first find the last occurence of slash and form a substring
     *    from it.*/
    size_t pos = cmdWithPath.rfind("/");
    if (pos != std::string::npos) {
        return cmdWithPath.substr(0, pos + 1);
    }
    return "-1";
}
std::string getFileName(std::string cmdWithPath){
    /* -Function: utilize rfind to get the position of last "/" and 
     *    return appropriate substring. */
    size_t pos = cmdWithPath.rfind("/");
    if (pos != std::string::npos) {
        return cmdWithPath.substr(pos + 1, std::string::npos);
    }
    return "-1";
}
std::vector<std::string> getPaths(){
    /* This function get the paths in the environment variable. */
   const char * paths = getenv("PATH");
   std::string pathsStr(paths);
   std::vector<std::string> vecPaths = parseLine(pathsStr, ":");
   return vecPaths;
}
std::vector<std::string> cmdsFromGetPaths(std::string cmd,
                               std::vector<std::string> paths) {
    /* -Function: this function forms potential commands from the paths
     *    derived from getPaths() so that we may try it with execve once a time.
     *
     * Note: This function should only be called while cmd, thus we put
     *    assert here to check this invariant.
     * */
    assert(hasSlash(cmd) == false);
    std::vector<std::string> potentialCmds;
    for (size_t i = 0; i < paths.size(); ++i) {
        std::string mergedPath = paths[i] + "/" + cmd;
        potentialCmds.push_back(mergedPath);
    }
    return potentialCmds;
}
/* End of Helper functions of myExecve */

/* Shell Runnning */
bool isEnd(std::vector<std::string> parsedArgs){
    /* -Function: This function first checks whether the user only types
     *    one input and the input is exactly "exit."
     * -Note:EXIT_MYSHELL is a macro "exit"
     *  */

    if ((parsedArgs.size() == 1) && (parsedArgs[0] == EXIT_MYSHELL)) {
        return true;
    } else {
        return false;
    }
}
void printShell(void){
    /* Use the system call get_current_dir_name() to print the shell. */
    std::string MYSHELL ("myShell:");
    char * currPath = get_current_dir_name();
    std::cout << MYSHELL + currPath + " $";
    free(currPath);
}
int myExecve(std::vector<std::string> parsedArgs, char ** envp) {
    /*  Function: This function first checks whether the command
     *      contains slash.
     *
     * Input : [std::vector<std::string>] parsedArgs, [char **] envp
     * Subroutines : execve(), hasSlash(), relativeToAbsolute()
     *      vecToCharStarStar(), free_vecToCharStarStar(), cmdsFromGetPaths()
     * */
    if (hasSlash(parsedArgs[0])) {
        /* if the command contains "/" */
        std::vector<std::string> updatedArgs = relativeToAbsolute(parsedArgs);
        char ** charArrArgs = vecToCharStarStar(updatedArgs);
        execve(updatedArgs[0].c_str(), charArrArgs, envp);
        exit(EXIT_FAILURE);
    } else {
        /* Don't contains /*/
        std::vector<std::string> potentialCmds = cmdsFromGetPaths(parsedArgs[0], getPaths());
        char ** charArrArgs = vecToCharStarStar(parsedArgs);
        for (size_t i = 0; i < potentialCmds.size(); ++i) {
            free(charArrArgs[0]);
            charArrArgs[0] = strdup(potentialCmds[i].c_str());
            if (execve(potentialCmds[i].c_str(), charArrArgs, envp) == -1) {
                continue;
            }
        }
        free_vecToCharStarStar(charArrArgs);
        perror("Cannot find commands");
        exit(EXIT_FAILURE);
    }
}
int runCommand(std::vector<std::string> parsedArgs) {
    /* -Function: This function mainly adopts the code in the man page
     *      of waitpid. It calls myExecve() and would run the command if 
     *      the command is found. Note if the command is not found, the 
     *      children process would be killed by exit(EXIT_FAILURE). 
     *
     * Input : [std::vector<std::string>] parsedArgs
     * Subroutine : myExecve()
     **/
    pid_t cpid;
    pid_t w;
    int statusRunCommand;
    cpid = fork();

    if (cpid == -1) {
       perror("Something wrong with fork.");
       exit(EXIT_FAILURE);
    }
    if (cpid == 0) {            /* Code executed by child */
        myExecve(parsedArgs, NULL);
        exit(EXIT_FAILURE);
    } else {                    /* Code executed by parent */
        do {
           w = waitpid(cpid, &statusRunCommand, WUNTRACED | WCONTINUED);
           if (w == -1) {
               perror("waitpid");
               exit(EXIT_FAILURE);
           }
           if (WIFEXITED(statusRunCommand)) {
               printf("Program exited with status %d\n", WEXITSTATUS(statusRunCommand));
           } else if (WIFSIGNALED(statusRunCommand)) {
               printf("Program was killed by signal %d\n", WTERMSIG(statusRunCommand));
           } else if (WIFSTOPPED(statusRunCommand)) {
               printf("stopped by signal %d\n", WSTOPSIG(statusRunCommand));
           } else if (WIFCONTINUED(statusRunCommand)) {
               printf("continued\n");
           }
        } while (!WIFEXITED(statusRunCommand) && !WIFSIGNALED(statusRunCommand));
    return EXIT_SUCCESS;
    }
}
int runShell(std::istream & streamRead){
    /* -Function: This is the main function that utilizes all other
     *      previous defined functions. We first call printShell() to
     *      indicate the shell would accept input. Then, it tackles the input
     *      from user for 4 ways.
     *         - If user types only while spaces or no input, we just
     *          call printShell() again.
     *         - Else if user types "exit", we ends the shell.
     *         - Else if user types any command in pushd, popd, dirstack,
     *          We call the corresponding function.
     *         - For the rest of commands, we call runCommand such that
     *          fisrt we check whether there is / and then try each
     *          possible combination of paths in execve().
     *
     * Input : [std::istream] std::cin
     * Subroutines: parseLine(), printShell(), isEnd(), isDirCall(),
     *           dirCall(), runCommand(), std::getline()
     * */
    std::string strInput;
    std::vector<std::string> parsedArgs;
    std::list<std::string> dirSTACK;

    printShell();
    while(std::getline(streamRead, strInput)){
        parsedArgs = parseLine(strInput, " ");
        if (parsedArgs.size() == 0) {
            printShell();
            continue;
        }
        if (isEnd(parsedArgs)) {
            dirSTACK.clear();
            return EXIT_SUCCESS;
        } else if (isDirCall(parsedArgs[0])) {
            dirCall(parsedArgs, dirSTACK) ;
        } else { // Be sure that special commands are run beforehand.
            runCommand(parsedArgs);
        }
        printShell();
    }
    return EXIT_SUCCESS;
}
int main(int argc, char ** argv) {
    if (argc > 1) {
        perror("Wrong number of input for myShell");
        return EXIT_FAILURE;
    }
    runShell(std::cin);
    return EXIT_SUCCESS;
}
