/**
 * critical_concurrency
 * CS 341 - Fall 2023
 */
#include "barrier.h"

// The returns are just for errors if you want to check for them.
int barrier_destroy(barrier_t *barrier) {
    pthread_mutex_destroy(&barrier->mtx);
    pthread_cond_destroy(&barrier->cv);
    return 0;
}

int barrier_init(barrier_t *barrier, unsigned int num_threads) {
    if (pthread_mutex_init(&barrier->mtx,NULL) != 0) {
        return pthread_mutex_init(&barrier->mtx,NULL);
    }
    if (pthread_cond_init(&(barrier->cv), NULL) != 0) {
        return pthread_cond_init(&(barrier->cv), NULL);
    }
    barrier->count = 0;
    barrier->n_threads = num_threads;
    barrier->times_used = 0;
    return 0;
}

int barrier_wait(barrier_t *barrier) {
    pthread_mutex_lock(&barrier->mtx);
    while (barrier->times_used != 0) {
      pthread_cond_wait(&barrier->cv, &barrier->mtx);
    }
    barrier->count += 1;
    while (barrier->count < barrier->n_threads) {
        pthread_cond_wait(&barrier->cv, &barrier->mtx);
    }
    if (barrier->count == barrier->n_threads) {
        pthread_cond_broadcast(&(barrier->cv));
    } 
    barrier->times_used += 1;
    if (barrier->times_used == barrier->count) {
        barrier->times_used = 0;
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cv);
    }
    barrier->count = 0;
    pthread_cond_signal(&barrier->cv);

    pthread_mutex_unlock(&(barrier->mtx));

    return 0;
}
