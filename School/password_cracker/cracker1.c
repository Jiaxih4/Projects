/**
 * password_cracker
 * CS 341 - Fall 2023
 */
#include "cracker1.h"
#include "format.h"
#include "utils.h"
#include "./includes/queue.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <crypt.h>

static queue* q;
static int recover = 0;
static int failed = 0;
static int count = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

typedef struct work{
    char* name;
    char* hash;
    char* pass;
} work;

void free_work(work* w);
int foundpassword(char* hash, char* crypt_hash);
int passwordnotfound(int hash_count);
void fillq(char* line, size_t len);
void makework(char* line, work* w);
void makethreads(size_t thread_count, pthread_t *tid);
void jointhreads(size_t thread_count, pthread_t *tid);
void doinput(char* line, size_t len);
void dothreads(size_t thread_count, pthread_t* tid);
void decrypt(int* pair, char* crypt_hash, work* temp, struct crypt_data cd, char* dotpoint);
void* helper(void* id);

int start(size_t thread_count) {
    // TODO your code here, make sure to use thread_count!
    // Remember to ONLY crack passwords in other threads
    pthread_t tid[thread_count];
    q = queue_create(-1);
    char* line = NULL;
    size_t len = 0;
    doinput(line, len);
    dothreads(thread_count, tid);
    v1_print_summary(recover,failed);
    pthread_mutex_destroy(&m);
    queue_destroy(q);
    return 0; // DO NOT change the return code since AG uses it to check if your
              // program exited normally
}

void fillq(char* line, size_t len){
    while((getline(&line, &len, stdin)) != -1) {
        work* w = calloc(1, sizeof(work));
        makework(line, w);
        queue_push(q, w);
        count++;
    }
    free(line);
    line = NULL;
}
void makework(char* line, work* w) {
    char n[39];
    char h[39];
    char p[39];
    sscanf(line, "%s %s %s", n, h, p);
    w->name = strdup(n);
    w->hash = strdup(h);
    w->pass = strdup(p);
}

int foundpassword(char* hash, char* crypt_hash) {
    if  (!strcmp(crypt_hash, hash)){
        pthread_mutex_lock(&m);
        recover++;
        pthread_mutex_unlock(&m);     
        return 1;
    }
    return 0;
}
int passwordnotfound(int hash_count) {
    if (hash_count == 456976){
        pthread_mutex_lock(&m);
        failed++;
        pthread_mutex_unlock(&m);
        return 1;
    }
    return 0;
}

void free_work(work* w) {
    free(w->name);
    free(w->hash);
    free(w->pass);
    free(w);
}

void makethreads(size_t thread_count, pthread_t* tid) {
    for (size_t i = 0; i < thread_count; i++) {
        pthread_create(&(tid[i]),NULL, helper, (void*)(i+1));
    }
}

void jointhreads(size_t thread_count, pthread_t* tid) {
    for(size_t i = 0; i < thread_count; i++){
        pthread_join(tid[i], NULL);
    }
}

void doinput(char* line, size_t len) {
    fillq(line, len);
}

void dothreads(size_t thread_count, pthread_t *tid) {
    makethreads(thread_count, tid);
    jointhreads(thread_count, tid);
}


void decrypt(int* pair, char* crypt_hash, work* temp, struct crypt_data cd, char* dotpoin) {
    while(1){
        crypt_hash = crypt_r(temp->pass, "xx", &cd);
        pair[0]++;
        if (foundpassword(temp->hash, crypt_hash) == 1) {
            pair[1] = 0;
            break;
        }
        incrementString(dotpoin);
        if (passwordnotfound(pair[0]) == 1) {
            break;
        }
    }
}

void* helper(void* id){
    int tid = (long) id;
    pthread_mutex_lock(&m);
    while(count != 0){
        work* temp = queue_pull(q);
        count--;
        struct crypt_data cd;
        cd.initialized = 0;
        pthread_mutex_unlock(&m);
        v1_print_thread_start(tid, temp->name);
        double time_cpu = getThreadCPUTime();
        char* dotpoin = temp->pass + getPrefixLength(temp->pass);
        setStringPosition(dotpoin, 0);
        int pair[2];
        pair[0] = 0;
        pair[1] = 1;
        char* crypt_hash = NULL;
        decrypt(pair, crypt_hash, temp, cd, dotpoin);
        pthread_mutex_lock(&m);
        v1_print_thread_result(tid,temp->name,temp->pass,pair[0],getThreadCPUTime()-time_cpu,pair[1]);
        free_work(temp);
    }
    pthread_mutex_unlock(&m);
    return NULL;
}