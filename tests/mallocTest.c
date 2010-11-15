#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char *block = malloc(sizeof(char) * 1024);
	printf("---- mallocTest: block address: %p\n", block);
	free(block);
	return 0;
}

