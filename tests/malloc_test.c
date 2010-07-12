#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int *data = malloc(sizeof(int));
    printf("**** %s: Allocated 4-byte chunk at %p.\n", argv[0], data);
    free(data);
    return 0;
}
