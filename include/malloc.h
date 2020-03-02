/*
** EPITECH PROJECT, 2020
** PSU_2019_malloc
** File description:
** lib header file
*/

#ifndef MALLOC_H_
#define MALLOC_H_

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

typedef enum alloc_state {
    UNKNOWN,
    FREE,
    ALLOC
} alloc_state_t;

typedef struct alloc_node_s {
    alloc_state_t state;
    size_t size;
    struct alloc_node_s *next;
} alloc_node_t;

#define get_header(x) ((alloc_node_t *)((size_t)x - sizeof(alloc_node_t)))
#define skip_header(x) ((size_t)x + sizeof(alloc_node_t))

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void *calloc(size_t nmemb, size_t size);
void *reallocarray(void *ptr, size_t nmemb, size_t size);
void free(void *ptr);

extern void *heap_head;
extern pthread_mutex_t mut_thread;

#endif /* !MALLOC_H_ */
