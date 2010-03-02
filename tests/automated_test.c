#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int *blocks[5];

int main(int argc, char *argv[]) {
    int x = 0;
    for(; x < 5; x ++) {
        blocks[x] = malloc(sizeof(int));
        sleep(1);
    }
    sleep(1);
    for(x = 0; x < 5; x ++) {
        free(blocks[x]);
        sleep(1);
    }
    return 0;
}

