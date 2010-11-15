#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>

void __attribute__((constructor)) AC_Construct() {
	printf("cpuTime constructing . . .\n");
}

void __attribute__((destructor)) AC_Destruct() {

}

void *malloc(size_t size) {
	return NULL;
}
