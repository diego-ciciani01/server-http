#include "malloc_utils.h"
#include <unistd.h>
#include <stdio.h>

void *safeMalloc(size_t bytes)
{
    void *result = malloc(bytes);
    if ( result == NULL){
        fprintf(stderr, "ERROR: malloc %zu\n", bytes);
        exit(1);
    }
    return result;
}
