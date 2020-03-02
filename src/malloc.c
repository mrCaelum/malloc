/*
** EPITECH PROJECT, 2020
** PSU_2019_malloc
** File description:
** malloc
*/

#include "malloc.h"

void *heap_head = NULL;
pthread_mutex_t mut_thread = PTHREAD_MUTEX_INITIALIZER;

static alloc_node_t *prev(alloc_node_t *ptr)
{
    for (alloc_node_t *current = heap_head; current; current = current->next)
        if (current->next == ptr)
            return current;
    return NULL;
}

static alloc_node_t *end(void)
{
    alloc_node_t *ptr = heap_head;

    if (!ptr)
        return NULL;
    while (ptr->next)
        ptr = ptr->next;
    return ptr;
}

static alloc_node_t *get_next_free(size_t size)
{
    alloc_node_t *optimal = NULL;
    alloc_node_t *tmp_ptr;
    size_t tmp_size;

    for (alloc_node_t *current = heap_head; current; current = current->next) {
        if (current->state == FREE && current->size == size)
            return current;
        if (current->state == FREE && current->size > size + sizeof(alloc_node_t) + 1 && (!optimal || optimal->size > current->size))
            optimal = current;
    }
    if (optimal == NULL)
        return NULL;
    tmp_ptr = optimal->next;
    tmp_size = optimal->size;
    optimal->size = size;
    optimal->next = (void *)(skip_header(optimal) + size);
    optimal->next->next = tmp_ptr;
    optimal->next->size = tmp_size - (size + sizeof(alloc_node_t));
    optimal->next->state = FREE;
    return optimal;
}

static alloc_node_t *mv_optimal_size(size_t size)
{
    size_t i = 1;
    alloc_node_t *ptr;
    alloc_node_t *tmp;

    while (i * sysconf(_SC_PAGESIZE) < size + 2 * sizeof(alloc_node_t) + 1)
        ++i;
    ptr = sbrk(i * sysconf(_SC_PAGESIZE));
    tmp = end();
    if (tmp)
        tmp->next = ptr;
    if (heap_head == NULL)
        heap_head = ptr;
    ptr->state = ALLOC;
    ptr->size = size;
    ptr->next = (void *)(skip_header(ptr) + size);
    ptr->next->state = FREE;
    ptr->next->size =
    i * sysconf(_SC_PAGESIZE) - (size + 2 * sizeof(alloc_node_t));
    ptr->next->next = NULL;
    return ptr;
}

void *malloc(size_t size)
{
    alloc_node_t *ptr = NULL;

    pthread_mutex_lock(&mut_thread);
    if (size == 0)
        return NULL;
    size += size % 2;
    if (heap_head != NULL)
        ptr = get_next_free(size);
    if (ptr) {
        ptr->state = ALLOC;
        pthread_mutex_unlock(&mut_thread);
        return (void *)skip_header(ptr);
    }
    ptr = mv_optimal_size(size);
    pthread_mutex_unlock(&mut_thread);
    return (void *)skip_header(ptr);
}
