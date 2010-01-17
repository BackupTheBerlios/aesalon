#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("**** sleep_test: about to sleep for three seconds . . .\n");
    sleep(3);
    printf("**** sleep_test: slept for three seconds, exiting . . .\n");
    return 0;
}
