#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int size = 1;
    char input;
    char *data = 0;
    char buffer[1024];
    while(1) {
        printf("**** How much memory to allocate (0 to quit)?\n");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%i", &size);
        if(!size) break;
        data = malloc(size);
        printf("**** Memory allocated -- enter 'y' to overwrite data, anything else otherwise.\n");
        fgets(buffer, sizeof(buffer), stdin);
        if(buffer[0] != 'y') {
            free(data);
            printf("**** Memory freed. Continuing.\n");
            data = 0;
        }
    }
    return 0;
}
