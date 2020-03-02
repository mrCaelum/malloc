/*
** EPITECH PROJECT, 2020
** PSU_2019_malloc
** File description:
** free
*/

#include "malloc.h"

void free(void *ptr)
{
    alloc_node_t *tmp;

    if (ptr == NULL)
        return;
    if (get_header(ptr)->state != ALLOC)
        return;
    pthread_mutex_lock(&mut_thread);
    get_header(ptr)->state = FREE;
    if (get_header(ptr)->next && get_header(ptr)->next->state == FREE) {
        get_header(ptr)->size +=
        sizeof(alloc_node_t) + get_header(ptr)->next->size;
        get_header(ptr)->next = get_header(ptr)->next->next;
    }
    pthread_mutex_unlock(&mut_thread);
}
