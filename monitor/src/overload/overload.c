/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file overload/overload.c
*/

#include <stdio.h>
#include <malloc.h>

void aesalon_initialize_overload();
void *aesalon_malloc_hook(size_t size, const void *caller);
void *aesalon_realloc_hook(void *ptr, size_t size, const void *caller);
void aesalon_free_hook(void *ptr, const void *caller);

void *(*aesalon_old_malloc_hook)(size_t size, const void *caller);
void *(*aesalon_old_realloc_hook)(void *ptr, size_t size, const void *caller);
void (*aesalon_old_free_hook)(void *ptr, const void *caller);
void (*__malloc_initialize_hook)(void) = aesalon_initialize_overload;


void aesalon_initialize_overload() {
    aesalon_old_malloc_hook = __malloc_hook;
    __malloc_hook = aesalon_malloc_hook;
    aesalon_old_realloc_hook = __realloc_hook;
    __realloc_hook = aesalon_realloc_hook;
    aesalon_old_free_hook = __free_hook;
    __free_hook = aesalon_free_hook;
}

void *aesalon_malloc_hook(size_t size, const void *caller) {
    void *data;
    __malloc_hook = aesalon_old_malloc_hook;
    data = malloc(size);
    __malloc_hook = aesalon_malloc_hook;
    
    /* Send off a SIGTRAP, this data is ready for collection . . . */
    asm("int3");
    
    return data;
}

void *aesalon_realloc_hook(void *ptr, size_t size, const void *caller) {
    void *data;
    __realloc_hook = aesalon_old_realloc_hook;
    data = realloc(ptr, size);
    __realloc_hook = aesalon_realloc_hook;
    
    /* Send off a SIGTRAP, this data is ready for collection . . . */
    asm("int3");
    
    return data;
}

void aesalon_free_hook(void *ptr, const void *caller) {
    __free_hook = aesalon_old_free_hook;
    free(ptr);
    __free_hook = aesalon_free_hook;
    
    asm("int3");
}
