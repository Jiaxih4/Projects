/**
 * password_cracker
 * CS 341 - Fall 2023
 */
#include "cracker2.h"
#include "format.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <unistd.h>
#include <stdlib.h>

pthread_barrier_t barrier_0, barrier_1;
static char *uname = NULL;
static char *hash = NULL;
static char *pass = NULL;
static char *pword = NULL;
pthread_mutex_t m;

static int ffh[4];

void setpos(int ti, long* pair, char* pws);
void free_all();
void lockprintthreadresult(int ti, int hcount, int result);
int iffound(char* hh, char* crypt_hash, char* pws,int ti, int hcount);
int afterfound(int ti, int hcount);
void decryptor(long* pair, char* pws, struct crypt_data cd, char* crypt_hash, int ti, int hcount, char* hh);
void init_ffh(int i);
void *helper(void *i);
void create_thread(pthread_t* tid);
void join_thread(pthread_t* tid);
void initstaticchar();

int start(size_t thread_count) {
    // TODO your code here, make sure to use thread_count!
    // Remember to ONLY crack passwords in other threads
    init_ffh(0);
    ffh[3] = (int)thread_count;
    pthread_mutex_init(&m, NULL);
    pthread_barrier_init(&barrier_0, NULL, ffh[3] + 1);
	  pthread_barrier_init(&barrier_1, NULL, ffh[3] + 1);
    pthread_t tid[ffh[3]];
    create_thread(tid);
    initstaticchar();
    char *line = NULL;
    size_t len = 0;
    while ((getline(&line,&len, stdin)) != -1) {
      sscanf(line, "%s %s %s", uname, hash, pass);
      pthread_barrier_wait(&barrier_0);
      double stime = getTime();
      double ctime = getCPUTime();
      v2_print_start_user(uname);
      pthread_barrier_wait(&barrier_1);
      v2_print_summary(uname, pword, ffh[2], getTime() - stime, getCPUTime() - ctime, !ffh[1]);
      pthread_mutex_lock(&m);
      ffh[1] = 0;
      ffh[2] = 0;
      pthread_mutex_unlock(&m);
    }
    ffh[0] = 1;
    pthread_barrier_wait(&barrier_0);
    free_all();
    free(line);
    join_thread(tid);
    pthread_mutex_destroy(&m);
    pthread_barrier_destroy(&barrier_0);
    pthread_barrier_destroy(&barrier_1);
    return 0; // DO NOT change the return code since AG uses it to check if your
              // program exited normally
}

void init_ffh(int i) {
    ffh[0] = i;
    ffh[1] = i;
    ffh[2] = i;
    ffh[3] = i;
}

void join_thread(pthread_t* tid) {
  for (int j = 0; j < ffh[3]; j++) {
    pthread_join(tid[j], NULL);
  }
}

void create_thread(pthread_t* tid) {
  for (size_t i = 0; i < (size_t)ffh[3]; i++) {
    pthread_create(tid+i, NULL, helper, (void *)(i+1));
  }
}

void *helper(void *id) {
  struct crypt_data cd;
  cd.initialized = 0;
  int ti = (long) id;
  char *pws = calloc(39, sizeof(char));
  char *crypt_hash = calloc(39, sizeof(char)); // hash
  while (1) {
    pthread_barrier_wait(&barrier_0);
    if (ffh[0]) {
      break;
    }
    long pair[2];
    pair[0] = 0;
    pair[1] = 0;
    int hcount = 0;
    char *hh = NULL;
    setpos(ti, pair, pws);
    v2_print_thread_start(ti, uname, pair[0], pws);
    strcpy(crypt_hash, hash); // hash
    decryptor(pair, pws, cd, crypt_hash, ti, hcount, hh);
    if (!ffh[1]) {
      lockprintthreadresult(ti, hcount, 2);
    }
    pthread_barrier_wait(&barrier_1);
  }
  free(pws);
  free(crypt_hash);
  return NULL;
}

void decryptor(long* pair, char* pws, struct crypt_data cd, char* crypt_hash, int ti, int hcount, char* hh) {
  for (long i = 0; i < pair[1]; i++) {
      hh = crypt_r(pws, "xx", &cd);
      hcount++;
      if (iffound(hh, crypt_hash, pws, ti, hcount)) {
        ffh[1] = 1;
        break;
      }
      if (afterfound(ti, hcount)) {
        break;
      }
      incrementString(pws);
    }
}

int afterfound(int ti, int hcount) {
  if (ffh[1]) {
    lockprintthreadresult(ti, hcount, 1);
    return 1;
  }
  return 0;
}

void initstaticchar() {
  uname = calloc(39, sizeof(char));
  hash = calloc(39, sizeof(char));
  pass = calloc(8, sizeof(char));
  pword = calloc(39, sizeof(char));
}

int iffound(char* hh, char* crypt_hash, char* pws,int ti, int hcount) {
  if (!strcmp(hh, crypt_hash)) {
      strcpy(pword, pws);
      lockprintthreadresult(ti, hcount, 0);
      return 1;
    }
  return 0;
}

void lockprintthreadresult(int ti, int hcount, int result) {
  pthread_mutex_lock(&m);
  v2_print_thread_result(ti, hcount, result);
  ffh[2] += hcount;
  pthread_mutex_unlock(&m);
}

void setpos(int ti, long* pair, char* pws) {
  int expose = getPrefixLength(pass);
  getSubrange(8-expose, ffh[3], ti, &(pair[0]), &(pair[1]));
  strcpy(pws, pass);
  setStringPosition(pws+expose, pair[0]);
}

void free_all() {
  free(uname);
  free(hash);
  free(pass);
  free(pword);
}