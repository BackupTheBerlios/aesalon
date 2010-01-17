#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *memory = 0;
    sleep(1);
    printf("**** sleep_malloc: about to allocate one byte of memory . . .\n");
    memory = malloc(1);
    sleep(1);
    printf("**** sleep_malloc: memory allocated (address is %p), freeing . . .\n", memory);
    free(memory);
    printf("**** sleep_malloc: memory freed, exiting . . .\n");
    sleep(1);
    memory = 0;
    return 0;
}
