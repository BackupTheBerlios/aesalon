#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *memory = 0;
    memory = malloc(1);
    free(memory);
    memory = 0;
    return 0;
}
