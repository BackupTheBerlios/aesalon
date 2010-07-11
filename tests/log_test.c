#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen("output.log", "at");
    fprintf(fp, "Opened log file.\n");
    fclose(fp);
    return 0;
}
