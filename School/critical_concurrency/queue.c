/**
 * critical_concurrency
 * CS 341 - Fall 2023
 */
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * This queue is implemented with a linked list of queue_nodes.
 */
typedef struct queue_node {
    void *data;
    struct queue_node *next;
} queue_node;

struct queue {
    /* queue_node pointers to the head and tail of the queue */
    queue_node *head, *tail;

    /* The number of elements in the queue */
    ssize_t size;

    /**
     * The maximum number of elements the queue can hold.
     * max_size is non-positive if the queue does not have a max size.
     */
    ssize_t max_size;

    /* Mutex and Condition Variable for thread-safety */
    pthread_cond_t cv;
    pthread_mutex_t m;
};

queue *queue_create(ssize_t max_size) {
    /* Your code here */
    queue *output = malloc(sizeof(struct queue));
    output->head = NULL;
    output->tail = NULL;
    output->size = 0;
    output->max_size = max_size;
    pthread_cond_init(&output->cv, NULL);
    pthread_mutex_init(&output->m, NULL);
    return output;
}

void queue_destroy(queue *this) {
    /* Your code here */
    if (this == NULL) {
        return;
    }
    queue_node *curr = this->head;
    queue_node * tmp = NULL;
    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    pthread_mutex_destroy(&this->m);
    pthread_cond_destroy(&this->cv);
}

void queue_push(queue *this, void *data) {
    /* Your code here */
    pthread_mutex_lock(&(this->m));
    while (this->max_size > 0 && this->size >= this->max_size) {
        pthread_cond_wait(&(this->cv), &(this->m));
    }
    queue_node *n = malloc(sizeof(queue_node));
    n->data = data;
    n->next = NULL;
    if (this->head == NULL && this->tail == NULL) {
        this->head = n;
        this->tail = n;
    } else {
        this->tail->next = n;
        this->tail = n;
    }
    this->size += 1;
    pthread_cond_broadcast(&this->cv);
    pthread_mutex_unlock(&this->m);
}

void *queue_pull(queue *this) {
    /* Your code here */
    pthread_mutex_lock(&this->m);
    while (this->head == NULL && this->tail == NULL) {
        pthread_cond_wait(&(this->cv), &(this->m));
    }
    void* output = this->head->data;
    queue_node *tmp = this->head;
    this->head = this->head->next;
    free(tmp);
    if (this->head == NULL) {
        this->tail = NULL;
    }
    pthread_cond_broadcast(&this->cv);
    pthread_mutex_unlock(&this->m);
    return output;
}
