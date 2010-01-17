#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *memory = 0;
    printf("**** malloc_test: about to allocate one byte of memory . . .\n");
    memory = malloc(1);
    printf("**** malloc_test: memory allocated (address is %p), freeing . . .\n", memory);
    free(memory);
    printf("**** malloc_test: memory freed, exiting . . .\n");
    memory = 0;
    return 0;
}
