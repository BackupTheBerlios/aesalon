#include <stdio.h>

void __attribute__((constructor)) Construct() {
	printf("cpuTime constructing . . .\n");
}

void __attribute__((destructor)) Destruct() {

}

