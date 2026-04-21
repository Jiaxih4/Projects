/**
 * parallel_make
 * CS 341 - Fall 2023
 */

#include "format.h"
#include "graph.h"
#include "parmake.h"
#include "parser.h"
#include "./includes/set.h"
#include "./includes/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
pthread_cond_t graph_cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t graph_lock = PTHREAD_MUTEX_INITIALIZER;

graph *g = NULL;
queue *q = NULL;
set *s = NULL;
set *wentto = NULL;
set *vpush = NULL;

int switches[5];

bool checkneighborsforcycle(vector* neighbors);
bool ifneighcyc(vector* neighbors, size_t i);
bool neighborscycle(char* target);
void clearoutset(set* v);
bool circledetector(char* target);
bool neighborscycle(char* target);
int noncycu(int target);
bool ifneighcyc(vector* neighbors, size_t i);
void addqtoque_dependency(vector* dd, char* target);
void changerule(char* target, vector* dd);
bool newtime(struct stat is, rule_t* rt);
void eachtime(vector* dependencies, struct stat is);
int should_run(void *target);
void workondepend();
void addqtoque(char *target);
bool comptime(struct stat st, struct stat is);
int changeintime(struct stat st, struct stat is, rule_t* rt);
void pushifstate(rule_t* r, char* vt);
void changestate(char* vt);
void addnotes(char* target, int sc);
int checkifcomplete(char* vt);
void checktoexecute(rule_t* rule);
void* helper();
int checkforcycle(char* str);
void checkthroughforcycle(vector *vv);
void avoidwarning(int num_threads);

void destroytheworld() {
    graph_destroy(g);
    queue_destroy(q);
    set_destroy(s);
}

int parmake(char *makefile, size_t num_threads, char **targets) {
    // good luck!
    switches[0] = 99999;
    switches[1] = 0;
    switches[2] = 1;
    switches[3] = 0;
    switches[4] = 0;
    avoidwarning(num_threads);
    pthread_t tid[num_threads];
    g = parser_parse_makefile(makefile, targets);
    q = queue_create(-1);
    s = string_set_create();
    vector *vv =graph_neighbors(g, "");

    checkthroughforcycle(vv);

    rule_t *root =graph_get_vertex_value(g, "");
    root->state = vector_size(vv);
    if (vector_empty(vv)) {
        return 0;
    }
    VECTOR_FOR_EACH(vv, vt, {
        addqtoque(vt);
        }
    );

    for (size_t i = 0; i < num_threads; i++) {
      pthread_create(tid+i, NULL, helper, NULL);
    }

    pthread_mutex_lock(&graph_lock);
    while (switches[3] != (int)vector_size(vv)) {
        pthread_cond_wait(&graph_cv, &graph_lock);
    }
    pthread_mutex_unlock(&graph_lock);
    /*
    while (count != 0) {
        helper();
    }
    */
    for (size_t i = 0; i < num_threads + 1; i++) {
        queue_push(q, NULL);
    }
    for (size_t j = 0; j < num_threads; j++) {
      pthread_join(tid[j], NULL);
    }
    vector_destroy(vv);
    destroytheworld();
    pthread_cond_destroy(&graph_cv);
    pthread_mutex_destroy(&graph_lock);
    return 0;
}

void clearoutset(set* v) {
    set_destroy(v);
    v = NULL;
}

bool neighborscycle(char* target) {
    set_add(wentto, target);
    vector *neighbors = graph_neighbors(g, target);
    if(checkneighborsforcycle(neighbors)==true) {
        return true;
    }
    vector_destroy(neighbors);
    return false;
}

void changerule(char* target, vector* dd) {
    void* v = graph_get_vertex_value(g, target);
    rule_t *rule = (rule_t *) v;
    rule->state = vector_size(dd);
}

void pushifstate(rule_t* r, char* vt) {
    if (r->state == 0) {
        switches[1]++;
        queue_push(q, vt);
    }
}

void avoidwarning(int num_threads) {
    if (num_threads > 0) {
        switches[0] = 39;
    }
}
void changestate(char* vt){
    void* v = graph_get_vertex_value(g, vt);
    rule_t *r = v;
    r->state --;
    pushifstate(r, vt);
}

bool circledetector(char* target) {
    if (!wentto) {
      wentto = shallow_set_create();
    }
    if(set_contains(wentto, target)) {
        clearoutset(wentto);
        return true;
    }
    if (neighborscycle(target)== true) {
        return true;
    }
    set_destroy(wentto);
    wentto = NULL;
    return false;
}

void addqtoque(char *target) {
    if (vpush == NULL) {
      vpush = shallow_set_create();
    }
    if (set_contains(vpush, target)){
        return;
    }
    set_add(vpush, target);
    vector* dd = graph_neighbors(g, target);
    addqtoque_dependency(dd, target);
    changerule(target, dd);
    vector_destroy(dd);
}

bool ifneighcyc(vector* neighbors, size_t i) {
    if (circledetector(vector_get(neighbors, (int)i))) {
        vector_destroy(neighbors);
        return true;
    }
    return false;
}

bool newtime(struct stat is, rule_t* rt){
    struct stat st;
    if (changeintime(st, is, rt)) {
        switches[4] = 1;
        return true;
    }
    return false;
}

void eachtime(vector* dependencies, struct stat is) {
    VECTOR_FOR_EACH(dependencies, vt, {
        char *temp = graph_get_vertex_value(g, vt);
        if (access(temp, F_OK) == 0) {
            rule_t* rt = (rule_t*) temp;
            if (newtime(is, rt) == true) {
                break;
            }
        }
    });
}
int should_run(void *target) {
    struct stat is;
    switches[4] = 0;
    if (access((char*)target, F_OK) == 0) {
        pthread_mutex_lock(&graph_lock);
        vector *dependencies = graph_neighbors(g, target);
        pthread_mutex_unlock(&graph_lock);
        eachtime(dependencies, is);
        vector_destroy(dependencies);
    } else {
        switches[4] = 1;
    }
    return switches[4];
}

bool checkneighborsforcycle(vector* neighbors) {
    size_t vs = vector_size(neighbors);
    for (size_t i = 0; i < vs; i++) {
        if(ifneighcyc(neighbors, i)) {
            return true;
        }    
      }
    return false;
}

bool comptime(struct stat st, struct stat is) {
    if((st.st_mtime - is.st_mtime) > 0) {
        return false;
    }
    return true;
}

void* helper() {
    while(1) {
        void * v =  queue_pull(q);
        if (!v) {
            break;
        }
        char *target = (char *) v;
        //count--;
        int status = should_run(target);
        void * vg = graph_get_vertex_value(g, target);
        rule_t *rule = (rule_t *) vg;
        switches[2] = 1;
        if (status == 1) {
            checktoexecute(rule);
        }
        pthread_mutex_lock(&graph_lock);
        addnotes(target, switches[2]);
        pthread_mutex_unlock(&graph_lock);
        //}
    }
    return NULL;
}

int checkforcycle(char* str) {
    if (circledetector(str)) {
        print_cycle_failure(str);
        void* vg = graph_get_vertex_value(g, str);
        rule_t *rule = (rule_t*) vg;
        rule->state = -39;
        return 1;
    }
    return 0;
}

void workondepend(char* target) {
    vector* dd = graph_neighbors(g, target);
    addqtoque_dependency(dd, target);
    changerule(target, dd);
    vector_destroy(dd);
}

void addqtoque_dependency(vector* dd, char* target) {
    VECTOR_FOR_EACH(dd, vt, {
        addqtoque(vt);
        }
    );
    if (vector_empty(dd)) {
        switches[1]++;
        queue_push(q, target);
    }
}


int changeintime(struct stat st, struct stat is, rule_t* rt) {
    if (stat(rt->target, &st) == -1|| (comptime(st, is) == false)) {
        return 1;
    }
    return 0;
}

int noncycu(int target) {
    if (g == NULL)
        return -1;
    vector *ks = graph_vertices(g);
    size_t nk = vector_size(ks);
    for (size_t i = 0; i < nk; ++i) {
        int value = 0;
        value+= target;
    }
    int result = 39 + 152 + target;
    vector_destroy(ks);
    return result;
}

void addnotes(char* target, int sc){
    vector *v = graph_antineighbors(g, target);
    VECTOR_FOR_EACH(v, vt, {
        if (sc) {
          changestate(vt);
        }
        if (!strcmp(vt, "")) {
            switches[3]++;
            pthread_cond_signal(&graph_cv);
        }
    });
    vector_destroy(v);
}

int checkifcomplete(char* vt) {
    if (system(vt) != 0) {
        switches[2] = 0;
        return 1;
    }
    return 0;
}

void checktoexecute(rule_t* rule) {
    VECTOR_FOR_EACH(rule->commands, vt, {
        if (checkifcomplete(vt) == 1) {
            return;
        }
    });
}

void checkthroughforcycle(vector *vv){
    size_t svv = vector_size(vv);
    for (size_t i = 0;i < svv; i++) {
        char *str = vector_get(vv, i);
        if (checkforcycle(str) == 1) {
            vector_erase(vv, i);
        }
    }
}