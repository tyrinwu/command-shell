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
    if (argc != 3) {
        perror("XXXXXXXX");
        exit(EXIT_FAILURE);
    } else {
        unsigned x = atoi(argv[1]);
        unsigned y = atoi(argv[2]);

        printf("%d", (unsigned) power(x, y));
    }
    return 0;
}
