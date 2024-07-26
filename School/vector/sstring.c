/**
 * vector
 * CS 341 - Fall 2023
 */
#include "sstring.h"
#include "vector.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <string.h>

struct sstring {
    // Anything you want
    char* s;
};

sstring *cstr_to_sstring(const char *input) {
    // your code goes here
    sstring * ss = malloc(sizeof(sstring));
    size_t l = strlen(input);
    ss->s = malloc (l + 1);
    ss->s = strdup(input);
    return ss;
}

char *sstring_to_cstr(sstring *input) {
    // your code goes here
    char* i = input->s;
    size_t l = strlen(i);
    char* out = malloc(l + 1);
    out = strdup(i);
    out[l] = '\0';
    return out;
}

int sstring_append(sstring *this, sstring *addition) {
    // your code goes here
    char* c = this->s;
    char* a = addition->s;
    size_t lt = strlen(c);
    size_t la = strlen(a);
    this->s = realloc(this->s, lt + la + 1);
    strcpy(this->s + lt, a);
    return strlen(this->s);
}

vector *sstring_split(sstring *this, char delimiter) {
    // your code goes here
    vector *v = vector_create(string_copy_constructor, string_destructor, string_default_constructor);
    char* start = this->s;
    char* end = this->s;
    char* l = strdup(this->s);
    while (end < l + strlen(l)) {
        if (*end == delimiter) {
            *end = '\0';
            vector_push_back(v, start);
            start = end + 1;
        }
        end++;
    }
    return v;
}

int sstring_substitute(sstring *this, size_t offset, char *target,
                       char *substitution) {
    // your code goes here
    char* c = this->s + offset;
    char * index = strstr(c, target);
    if (index != NULL) {
        char* s = this->s;
        size_t l = strlen(s);
        size_t ls = strlen(substitution);
        size_t lt = strlen(target);
        size_t sum = l + ls - lt;
        char * tmp = malloc(sum + 1);
        size_t sub = index - s;
        tmp = strndup(this->s, sub);
        strcat(tmp + sub, substitution);
        strcat(tmp + sub + ls, index + lt);
        free(this->s);
        this->s = tmp;
        return 0;
    } 
    return -1;
}

char *sstring_slice(sstring *this, int start, int end) {
    // your code goes here
    int diff = end - start;
    char * c = malloc(diff + 1);
    c = strndup(this->s + start, diff);
    c[diff] = '\0';
    return c;
}

void sstring_destroy(sstring *this) {
    // your code goes here
    free(this->s);
    free(this);
}
