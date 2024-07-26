/**
 * deadlock_demolition
 * CS 341 - Fall 2023
 */
#include "graph.h"
#include "libdrm.h"
#include "set.h"
#include <pthread.h>

struct drm_t {pthread_mutex_t m;};
static graph* g;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
set *s;

int cycle(void* v) {
    if (!s) {
        s = shallow_set_create();
    }
    if (set_contains(s, v)) {
        s = NULL;
        return 1;
    }
    set_add(s, v);
    vector* n  = graph_neighbors(g, v);
    for (size_t i = 0; i < vector_size(n); i++) {
        if (cycle(vector_get(n, i))) {
            return 1;
        }
    }
    s = NULL;
    return 0;
}

drm_t *drm_init() {
    /* Your code here */
    drm_t *output = malloc(sizeof(drm_t));
    pthread_mutex_init(&(output->m), NULL);
    pthread_mutex_lock(&(m));
    if (!g) {
        g = shallow_graph_create();
    }
    graph_add_vertex(g, output);
    pthread_mutex_unlock(&(m));
    return output;
}

int drm_post(drm_t *drm, pthread_t *thread_id) {
    /* Your code here */
    pthread_mutex_lock(&m);
    int output = 0;
    if (!graph_contains_vertex(g, thread_id)) {
        pthread_mutex_unlock(&m);
        output = 0;
        return output;
    }
    if (!graph_contains_vertex(g, drm)) {
        pthread_mutex_unlock(&m);
        output = 0;
        return output;
    }
    if (graph_adjacent(g, drm, thread_id)) {
        graph_remove_edge(g, drm, thread_id);
        pthread_mutex_unlock(&drm->m);
    }
    pthread_mutex_unlock(&m);
    output = 1;
    return output;
}

int drm_wait(drm_t *drm, pthread_t *thread_id) {
    /* Your code here */
    int output = 0;
    int lev = -1;
    pthread_mutex_lock(&m);
    graph_add_vertex(g, thread_id);
    graph_add_edge(g, thread_id, drm);
    if (graph_adjacent(g, drm, thread_id) || cycle(thread_id)) {
        output = 0;
        if (graph_adjacent(g, drm, thread_id)) {
            lev = 0;
        } else if (cycle(thread_id)) {
            lev = 1;
        }
        if (lev == 0) {
            pthread_mutex_unlock(&(m));
        } else if (lev == 1) {
            graph_remove_edge(g, thread_id, drm);
            pthread_mutex_unlock(&m);
        }
        return output;

    } else {
      pthread_mutex_unlock(&m);
      pthread_mutex_lock(&drm->m);
      pthread_mutex_lock(&m);
      graph_remove_edge(g, thread_id, drm);
      graph_add_edge(g, drm, thread_id);
      pthread_mutex_unlock(&m);
      output = 1;
      return output;
    }
}

void drm_destroy(drm_t *drm) {
    /* Your code here */
    graph_remove_vertex(g, drm);
    pthread_mutex_destroy(&drm->m);
    free(drm);
    pthread_mutex_destroy(&m);
    return;
}
