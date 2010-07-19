#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    time_t start = time(NULL);

    while(time(NULL) < (start + 5)) ;
    sleep(5);
    start = time(NULL);
    while(time(NULL) < (start + 5)) ;

    return 0;
}
