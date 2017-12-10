#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <string>

int main(int argc, char ** argv) {
    std::string s;
    if (argc == 1) {
        while (std::getline(std::cin, s)) {
            std::string i = std::to_string(atoi(s.c_str())+1);
            std::cout << i << std::endl;
        }
    } else if (argc == 2) {
        std::string fname(argv[1]);
        std::ifstream fileNew(fname);
        while (std::getline(fileNew, s)) {
            std::string i = std::to_string(atoi(s.c_str())+1);
            std::cout << i << std::endl;
        }
        return EXIT_SUCCESS;
    } else {
        perror("Wrong number of arguments.");
        return EXIT_FAILURE;
    }
}
