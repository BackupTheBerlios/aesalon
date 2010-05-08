#include <stdio.h>
#include <stdlib.h>

char *buffers[128];
int buffer_sizes[128];
int allocated_buffers;
int allocated_size;

void allocated_buffer();
void release_buffer();

int finished;

void clear_terminal() {
    system("clear");
}

void allocate_buffer() {
    int size = 0;
    printf("How big of a block? (minimum 1 byte)\n");
    while(scanf("%i", &size)) {
        if(size < 1) {
            printf("Size must be at least one byte!\n");
            size = 0;
            continue;
        }
        if(size > 131072) {
            printf("Warning: block is over default MMAP_THRESHOLD.\n");
        }
        break;
    }
    if(size == 0) {
        printf("EOF detected, aborting allocation.\n");
        return;
    }
    allocated_size += size;
    allocated_buffers ++;
    int which = 0;
    for(; which < 128; which ++) {
        if(buffer_sizes[which] == 0) break;
    }
    /* NOTE: there will be at least one free slot. Don't worry about that case, in other words. */
    buffer_sizes[which] = size;
    buffers[which] = malloc(size);
}

void release_buffer() {
    int which = 0;
    printf("Free which block?\n");
    while(scanf("%i", &which)) {
        if(which < 1 || which > 128) {
            printf(". . . out of range. Try again.\n");
            which = 0;
            continue;
        }
        if(buffer_sizes[which-1] == 0) {
            printf(". . . block is not allocated. Try again.\n");
            which = 0;
            continue;
        }
        break;
    }
    if(which == 0) {
        printf("EOF detected, aborting free.\n");
        return;
    }
    which --;
    allocated_size -= buffer_sizes[which];
    buffer_sizes[which] = 0;
    free(buffers[which]);
    allocated_buffers --;
}

void leak_buffer() {
    int which = 0;
    printf("Leak which block?\n");
    while(scanf("%i", &which)) {
        if(which < 1 || which > 128) {
            printf(". . . out of range. Try again.\n");
            which = 0;
            continue;
        }
        if(buffer_sizes[which-1] == 0) {
            printf(". . . block is not allocated. Try again.\n");
            which = 0;
            continue;
        }
        break;
    }
    if(which == 0) {
        printf("EOF detected, aborting free.\n");
        return;
    }
    which --;
    allocated_size -= buffer_sizes[which];
    buffer_sizes[which] = 0;
    allocated_buffers --;
}


void display_stats() {
    int i = 0;
    clear_terminal();
    for(; i < 80; i ++) printf("_");
    printf("\n");
    printf("\tAllocated blocks: \t%i\n", allocated_buffers);
    printf("\tTotal memory allocated:\t%i\n", allocated_size);
    printf("\t\t    ");
    for(i = 0; i < 20; i ++) printf("-");
    printf(" Allocated blocks ");
    for(i = 0; i < 20; i ++) printf("-");
    printf("\n");
    
    for(i = 0; i < 128; i ++) {
        if(buffer_sizes[i]) {
            printf("\t[%i]: %i bytes at %p . . . \n", i + 1, buffer_sizes[i], buffers[i]);
        }
    }
    printf("\t\t");
    for(i = 0; i < 20; i ++) printf("-");
    printf(" End of allocated blocks list ");
    for(i = 0; i < 20; i ++) printf("-");
    printf("\n");
    printf("\n");
}

void display_actions() {
    int i = 0;
    int c;
    printf("\nDo what?\n");
    if(allocated_buffers < 128) printf("\t(A)llocate another block of memory\n");
    if(allocated_buffers > 0) printf("\t(F)ree a block of memory\n");
    if(allocated_buffers > 0) printf("\t(L)eak a block of memory\n");
    printf("\tE(x)it.\n");
    while((c = tolower(getchar()))) {
        if(allocated_buffers < 128 && c == 'a') {
            allocate_buffer();
            break;
        }
        if(allocated_buffers > 0 && c == 'f') {
            release_buffer();
            break;
        }
        if(allocated_buffers > 0 && c == 'l') {
            leak_buffer();
            break;
        }
        if(c == 'x') {
            finished = 1;
            break;
        }
        if(c != '\r') printf("Unknown command. Try again . . .\n");
    }
}

int main(int argc, char *argv[]) {
    printf("Starting . . .\n");
    while(!finished) {
        display_stats();
        display_actions();
    }
    return 0;
}
