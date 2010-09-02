#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *buffers[10];
    int i = 0;
    for(i = 0; i < 10; i ++) {
        buffers[i] = malloc(i * 2);
        usleep(100000);
    }
    for(i = 0; i < 10; i ++) {
        free(buffers[i]);
        usleep(100000);
    }
    return 0;
}
