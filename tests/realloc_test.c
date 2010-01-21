#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *memory = 0;
    printf("**** realloc_test: about to allocate one byte of memory . . .\n");
    memory = malloc(1);
    printf("**** realloc_test: memory allocated (address is %p), resizing . . .\n", memory);
    memory = realloc(memory, 2);
    printf("**** realloc_test: memory resized (new address is %p), freeing . . .\n", memory);
    free(memory);
    printf("**** realloc_test: memory freed, exiting . . .\n");
    memory = 0;
    return 0;
}
