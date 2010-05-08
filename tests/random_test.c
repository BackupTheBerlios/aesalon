#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *buffers[128];
int allocated[128];
int allocated_count;

int select_buffer(int looking_for) {
    int i = rand() % 128;
    while(allocated[i] != looking_for) i = rand() % 128;
    return i;
}

void alloc() {
    printf("allocating . . .\n");
    if(allocated_count == 128) return;
    int i = select_buffer(0);
    buffers[i] = malloc(sizeof(char) * (rand() % 16));
    allocated[i] = 1;
    allocated_count ++;
    printf("\tallocated %i\n", i);
}

void release() {
    printf("releasing . . .\n");
    if(allocated_count == 0) return;
    int i = select_buffer(1);
    free(buffers[i]);
    allocated[i] = 0;
    allocated_count --;
    printf("\treleased %i\n", i);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    while(1) {
        if(rand() % 2) alloc();
        if(rand() % 2) release();
        sleep(1);
    }
    return 0;
}
