/**
 * teaching_threads
 * CS 341 - Fall 2023
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "reduce.h"
#include "reducers.h"

/* You might need a struct for each task ... */
typedef struct red {
    reducer func;
    int* start;
    int* end;
    int out;
    int* list;
} red;
/* You should create a start routine for your threads. */

void* red_f(void* data) {
    red * r = (red*) data;
    for (int* i = r->start; i != r->end; i++) {
        r->out = r->func(r->out, *i);
    }
    pthread_exit(NULL);
}

int par_reduce(int *list, size_t list_len, reducer reduce_func, int base_case,
               size_t num_threads) {
    /* Your implementation goes here */
    size_t div = list_len / num_threads;
    pthread_t tid[num_threads];
    red *data = malloc(num_threads * sizeof(red));
    for (size_t i = 0; i < num_threads; i++) {
        data[i] = (red){reduce_func, (list + i*div), (list + (i+1)*div), base_case, list};
        pthread_create(tid+i, NULL, red_f, data+i);
        if (i == num_threads-1) {
            data[i].end = (list + list_len);
            for (size_t j = 0; j < num_threads; j++) {
                pthread_join(tid[j], NULL);
            }
        }
    }
    int l[num_threads];
    for (size_t i = 0; i < num_threads; i++) {
        l[i] = data[i].out;
    }
    int result = reduce(l, num_threads, reduce_func, base_case);
    return result;
}
