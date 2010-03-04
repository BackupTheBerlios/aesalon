#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_BLOCKS 10

int *blocks[NUM_BLOCKS];

int main(int argc, char *argv[]) {
    int x = 0;
    sleep(1);
    for(; x < NUM_BLOCKS; x ++) {
        blocks[x] = malloc(sizeof(int));
        printf("Allocated block at address %p . . .\n", blocks[x]);
        sleep(1);
    }
    sleep(1);
    for(x = 0; x < NUM_BLOCKS; x ++) {
        printf("Freeing block at %p . . .\n", blocks[x]);
        free(blocks[x]);
        sleep(1);
    }
    return 0;
}
