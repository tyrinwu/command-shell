#include "myShell.h"

void test_hasSlash() {
    std::string test0 = "";
    std::cout << "Should be 0 and get: " << hasSlash(test0) << std::endl;
    std::string test1 = "afasdf";
    std::cout << "Should be 0 and get: " << hasSlash(test1) << std::endl;
    std::string test2 = "/afasdf";
    std::cout <<  "Should be 1 and get: " << hasSlash(test2) << std::endl;
    std::string test3 = "afa/sdf";
    std::cout << "Should be 1 and get: " << hasSlash(test3) << std::endl;
    std::string test4 = "afasdfi/";
    std::cout << "Should be 1 and get: " << hasSlash(test4) << std::endl;
    std::string test5 = "/";
    std::cout << "Should be 1 and get: " << hasSlash(test5) << std::endl;
}
void test_parseLine() {
    std::vector<std::string> testStrings;
    testStrings.push_back("");
    testStrings.push_back("exit");
    testStrings.push_back(" exit");
    testStrings.push_back("exit ");
    testStrings.push_back(" exit ");
    testStrings.push_back("ls");
    testStrings.push_back("pwd");
    testStrings.push_back("echo a\\ \\ \\ \\ \\ b"); // should be <a           b>
    testStrings.push_back("echo I love You"); // should be <echo, I, love, you>
    testStrings.push_back("echo a\\ b c\\ \\ d"); // should have <a b,c  d>
    testStrings.push_back("echo a\\  b c \\ d"); // should have <a ,b,c, d>
    testStrings.push_back("./program a\\  b c \\ d"); // should have <a ,b,c, d>
    testStrings.push_back("/home/tw185/ece551/ a\\  b c \\ d"); // should have "a " "b" "c" " d"
    testStrings.push_back("/home/tw185/ece551/mp_miniproject/myShell a\\  b c \\ d"); // should have "a " "b" "c" " d"
    testStrings.push_back("ls this is the last time");

    std::vector<std::string>::iterator itTest = testStrings.begin();
    while (itTest != testStrings.end()) {
        std::vector<std::string> parsedStr = parseLine(*itTest, " ");
        std::vector<std::string>::iterator itParsed = parsedStr.begin();
        std::cout << "Original string: <" << *itTest << "> Parsed string: <";
        while (itParsed != parsedStr.end()) {
            if (itParsed != parsedStr.begin()){
                std::cout << ",";
            }
            std::cout << *itParsed;
            ++itParsed;
        }
        std::cout << ">" <<std::endl;
        ++itTest;
    }
}
void test_isEnd() {
    std::vector<std::string> testStrings;
    testStrings.push_back("exit");
    std::vector<std::string> testStrings2;
    testStrings2.push_back("exit");
    testStrings2.push_back("extraTerm");
    std::vector<std::string> testStrings3;
    testStrings3.push_back("exitt");
    //std::vector<std::string> testStrings3({"exit", "extraCommand"}); // ask

    std::cout << "Expect true, and get ("  << isEnd(testStrings)  << ")" << std::endl;
    std::cout << "Expect false, and get (" << isEnd(testStrings2) << ")" << std::endl;
    std::cout << "Expect false, and get (" << isEnd(testStrings3) << ")" << std::endl;
}

void test_vecToCharStarStar(){
    std::vector<std::string> testStrings2 = parseLine("echo $PATH", " ");
    //char ** result2 = new char*[testStrings2.size()+1];
    char ** result2 = vecToCharStarStar(testStrings2);
    for (size_t i = 0; i < testStrings2.size(); ++i) {
        std::cout << result2[i] << "\n";
    }
    free_vecToCharStarStar(result2);
}
void test_runCommand() {
    char * cwd = get_current_dir_name();
    std::cout << cwd << "\n";
    free(cwd);
    /* Testing the case that first argument does not contains "/" */
    std::vector<std::string> testStrings1 = parseLine("ls", " ");
    runCommand(testStrings1);
    std::cout << "-- Above should exit with status 0\n";
    std::cout << "####################################################\n";
    /* Testing the case that first argument does not contains "/" */
    std::vector<std::string> testStrings2 = parseLine("pwd", " ");
    runCommand(testStrings2);
    std::cout << "-- Above should exit with status 0\n";
    std::cout << "####################################################\n";
    /* Testing the case that first argument does contains "/" and the
     * path is relative*/
    std::vector<std::string> teststrings31 = parseLine("./dirTest/plus1 text1.txt", " ");
    runCommand(teststrings31);
    std::cout << "-- Above should exit with status 0\n";
    std::cout << "####################################################\n";
    /* Testing the case that first argument does contains "/" and the
     * path is relative. Also, the second argument is also relative */
    std::vector<std::string> teststrings32 = parseLine("./dirTest/plus1 ./dirTest/text1.txt", " ");
    runCommand(teststrings32);
    std::cout << "-- Above should exit with status 0\n";
    std::cout << "####################################################\n";
    /* Testing the case that first argument does contains "/" and the
     * path is absolute*/
    std::vector<std::string> teststrings4 = parseLine("/home/tw185/ece551/26_power_rec/power 2 3", " ");
    runCommand(teststrings4);
    std::cout << "-- Above should exit with status 0\n";
    std::cout << "####################################################\n";
    /* Testing the case that first argument does contains "/" and the
     * path is relative*/
    std::vector<std::string> teststrings5 = parseLine("../../26_power_rec/power 2 3", " ");
    runCommand(teststrings5);
    std::cout << "-- Above should exit with status 0\n";
    std::cout << "####################################################\n";
}

void test_getDir() {
    std::string dirName = getDir("../This/");
    std::cout << "Expecting ../This and get :" << dirName << std::endl;
    std::string dirName2 = getDir("/../This");
    std::cout << "Expecting /../ and get :" << dirName2 << std::endl;
    std::string dirName3 = getDir("/../This/");
    std::cout << "Expecting /../This/ and get :" << dirName3 << std::endl;
}
void test_getFileName() {
    std::string fName = getFileName("../This/");
    std::cout << "Expecting \"\" and get :" << fName << std::endl;
    std::string fName2 = getFileName("/../This");
    std::cout << "Expecting This and get :" << fName2 << std::endl;
    std::string fName3 = getFileName("/../This/dddddd");
    std::cout << "Expecting dddddd and get :" << fName3 << std::endl;
}
void test_getPaths() {
    std::vector<std::string> paths = getPaths();
    for (size_t i = 0; i < paths.size(); ++i) {
        std::cout << paths[i] << std::endl;
    }
}
void test_relativeToAbsolute() {
    std::vector<std::string> testStrings1 = parseLine("ls", " ");
    printParsedStr(relativeToAbsolute(testStrings1));
    std::cout << "####################################################\n";
    std::vector<std::string> testStrings2 = parseLine("pwd", " ");
    printParsedStr(relativeToAbsolute(testStrings2));
    std::cout << "####################################################\n";
    std::vector<std::string> teststrings3 = parseLine("./dirTest/plus1 ./dirTest/text1.txt", " ");
    printParsedStr(relativeToAbsolute(teststrings3));
    std::cout << "####################################################\n";
    std::vector<std::string> teststrings4 = parseLine("./dirTest/plus1 ../testMyShell/dirTest/text1.txt", " ");
    printParsedStr(relativeToAbsolute(teststrings4));
    std::cout << "####################################################\n";
    std::vector<std::string> teststrings5 = parseLine("./././dirTest/plus1 ../testMyShell/dirTest/text1.txt", " ");
    printParsedStr(relativeToAbsolute(teststrings5));
    std::cout << "####################################################\n";
    std::vector<std::string> teststrings6 = parseLine("../../mp_niniproject/testMyShell/./dirTest/plus1 ../testMyShell/dirTest/text1.txt", " ");
    printParsedStr(relativeToAbsolute(teststrings6));
    std::cout << "####################################################\n";
    std::vector<std::string> teststrings7 = parseLine("./dirTest/plus1 text1.txt", " ");
    printParsedStr(relativeToAbsolute(teststrings7));
    std::cout << "####################################################\n";
    std::vector<std::string> teststrings8 = parseLine("/home/tw185/ece551/26_power_rec/power 2 3", " ");
    printParsedStr(relativeToAbsolute(teststrings8));
    std::cout << "####################################################\n";
    std::vector<std::string> teststrings9 = parseLine("../../26_power_rec/power 2 3", " ");
    printParsedStr(relativeToAbsolute(teststrings9));
    std::cout << "####################################################\n";
}
void test_cmdsFromGetPaths(){
    std::string cmd = "pwd";
    std::vector<std::string> paths = getPaths();
    std::vector<std::string> test1 = cmdsFromGetPaths(cmd, paths);
    for (size_t i = 0; i <test1.size(); ++i ) {
        std::cout << test1[i] << std::endl;
    }
}
void test_runShell_STDCIN(){
    std::cout << "########################## Start Manual Testing ########################\n";
    runShell(std::cin);
    std::cout << "########################## End of Manual Testing ########################\n";
}
void testMain(){
    std::cout << "------------testing parseLine-------------" << std::endl;
    test_parseLine();
    std::cout << "------------testing isEnd-------------" << std::endl;
    test_isEnd();
    std::cout << "------------testing vecToCharStarStar-------------" << std::endl;
    test_vecToCharStarStar();
    std::cout << "------------testing getPaths-------------" << std::endl;
    test_getPaths();
    std::cout << "------------testing hasSlash-------------" << std::endl;
    test_hasSlash();
    std::cout << "------------testing getDir-------------" << std::endl;
    test_getDir();
    std::cout << "------------testing relativeToAbsolute-------------" << std::endl;
    test_relativeToAbsolute();
    std::cout << "------------testing getFileName-------------" << std::endl;
    test_getFileName();
    std::cout << "------------testing getPaths-------------" << std::endl;
    test_getPaths();
    std::cout << "------------testing cmdsFromGetPaths-------------" << std::endl;
    test_cmdsFromGetPaths();
    std::cout << "------------testing myExecve-------------" << std::endl;
    /* Did not run it here since if success it would exit the whole
     * program. Instead, I tested the behavior of myExecve in
     * test_runCommand*/
    //test_myExecve();
    std::cout << "------------testing runCommand-------------" << std::endl;
    test_runCommand();
    std::cout << "------------testing runShell-------------" << std::endl;
    /* Check manually what happens while user are typing.*/
    test_runShell_STDCIN();
}

int main(int argc, char ** argv, char ** envp) {
    testMain();
    return EXIT_SUCCESS;
}
