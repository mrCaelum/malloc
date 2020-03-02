/*
** EPITECH PROJECT, 2020
** PSU_2019_malloc
** File description:
** realloc
*/

#include "malloc.h"

static void *realloc_greater(void *ptr, size_t size)
{
    alloc_node_t tmp;
    alloc_node_t *new;

    if (get_header(ptr)->next && get_header(ptr)->next->state == FREE
    && get_header(ptr)->next->size > size - get_header(ptr)->size) {
        pthread_mutex_lock(&mut_thread);
        tmp.next = get_header(ptr)->next->next;
        tmp.size = get_header(ptr)->next->size;
        tmp.state = get_header(ptr)->next->state;
        get_header(ptr)->next = ptr + size;
        get_header(ptr)->next->state = tmp.state;
        get_header(ptr)->next->size =
        tmp.size - (size - get_header(ptr)->size);
        get_header(ptr)->next->next = tmp.next;
        get_header(ptr)->size = size;
        pthread_mutex_unlock(&mut_thread);
        return ptr;
    } else {
        new = malloc(size);
        if (!new)
            return NULL;
        memcpy(new, ptr, get_header(ptr)->size);
        free(ptr);
        return new;
    }
}

static void *realloc_smaller(void *ptr, size_t size)
{
    alloc_node_t *tmp;

    if (get_header(ptr)->size - size <= sizeof(alloc_node_t)) {
        tmp = malloc(size);
        if (!tmp)
            return NULL;
        memcpy(tmp, ptr, size);
        free(ptr);
    } else {
        pthread_mutex_lock(&mut_thread);
        tmp = get_header(ptr)->next;
        get_header(ptr)->next = (void *)((size_t)ptr + size);
        get_header(ptr)->next->next = tmp;
        get_header(ptr)->next->state = FREE;
        get_header(ptr)->next->size =
        get_header(ptr)->size - size - sizeof(alloc_node_t);
        get_header(ptr)->size = size;
        pthread_mutex_unlock(&mut_thread);
    }
    return tmp;
}

void *realloc(void *ptr, size_t size)
{
    if (ptr && !size) {
        free(ptr);
        return NULL;
    } else if (!ptr || !size)
        return malloc(size);
    if (get_header(ptr)->size == size)
        return ptr;
    if (get_header(ptr)->size < size) {
        return realloc_greater(ptr, size);
    } else {
        return realloc_smaller(ptr, size);
    }
}
