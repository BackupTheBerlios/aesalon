#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *memory = 0;
    printf("**** calloc_test: about to allocate an array of memory . . .\n");
    memory = calloc(10, 10);
    printf("**** calloc_test: memory allocated (address is %p), freeing . . .\n", memory);
    free(memory);
    printf("**** calloc_test: memory freed, exiting . . .\n");
    memory = 0;
    return 0;
}
