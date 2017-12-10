#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y) {
    if (y == 0) {
        return 1;
    } else {
        return power(x, y-1) * x;
    }
}


int main(int argc, char ** argv) {
    if (argc != 2) {
        perror("XXXXXXXX");
        exit(EXIT_FAILURE);
    } else {
        unsigned y = atoi(argv[1]);

        printf("%d\n", (unsigned) power(3, y));
    }
    return 0;
}
