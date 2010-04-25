#ifndef AESALON_OVERLOAD_COMMON_H
#define AESALON_OVERLOAD_COMMON_H

#include <stdint.h>

#define ALLOC_TYPE 1
#define ALLOC_DATA_SIZE ((sizeof(unsigned long) * 2) + sizeof(uint64_t))
typedef union {
    struct {
        uint64_t timestamp;
        unsigned long address;
        unsigned long size;
    } data;
    char buffer[ALLOC_DATA_SIZE];
} allocation_data_u;

#define REALLOC_TYPE 2
#define REALLOC_DATA_SIZE ((sizeof(unsigned long) * 3) + sizeof(uint64_t))
typedef union {
    struct {
        uint64_t timestamp;
        unsigned long original_address;
        unsigned long new_address;
        unsigned long new_size;
    } data;
    char buffer[REALLOC_DATA_SIZE];
} reallocation_data_u;

#define FREE_TYPE 3
#define FREE_DATA_SIZE ((sizeof(unsigned long) * 1) + sizeof(uint64_t))
typedef union {
    struct {
        uint64_t timestamp;
        unsigned long address;
    } data;
    char buffer[FREE_DATA_SIZE];
} free_data_u;
    
#endif