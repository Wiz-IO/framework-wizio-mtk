

//#include <stdio.h>
//#include <stdint.h>
//#include <stddef.h>
//#include <unistd.h>
//#include <time.h>
//#include <reent.h>
//#include <string.h>
#include "osal.h"

////////////////////////////////////////////////////////////////////////////
// HEAP
////////////////////////////////////////////////////////////////////////////

uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__((section(".heap")));

////////////////////////////////////////////////////////////////////////////
// MEMORY
////////////////////////////////////////////////////////////////////////////

void *malloc(size_t size)
{
    return pvPortMalloc(size);
}
void *_malloc_r(struct _reent *ignore, size_t size) { return malloc(size); }

void free(void *p)
{
    if (p)
        vPortFree(p);
}
void _free_r(struct _reent *ignore, void *ptr) { free(ptr); }

void *realloc(void *mem, size_t newsize)
{
    void *new = malloc(newsize);
    if ((new) && (mem))
        memcpy(new, mem, newsize);
    free(mem);
    return new;
}
void *_realloc_r(struct _reent *ignored, void *ptr, size_t size) { return realloc(ptr, size); }

void *calloc(size_t element, size_t size)
{
    size_t total = element * size;
    void *ret = malloc(total);
    if (ret)
        memset(ret, 0, total);
    return ret;
}
void *_calloc_r(struct _reent *ignored, size_t element, size_t size) { return calloc(element, size); }