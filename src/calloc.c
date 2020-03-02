/*
** EPITECH PROJECT, 2020
** PSU_2019_malloc
** File description:
** calloc
*/

#include "malloc.h"

void *calloc(size_t nmemb, size_t size)
{
    void *ptr;

    if ((size_t)(nmemb * size) / nmemb != size)
        return NULL;
    ptr = malloc(nmemb * size);
    if (!ptr)
        return NULL;
    memset(ptr, 0, nmemb * size);
    return ptr;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
    if ((size_t)(nmemb * size) / nmemb != size)
        return NULL;
    return realloc(ptr, nmemb * size);
}
