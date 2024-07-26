/**
 * vector
 * CS 341 - Fall 2023
 */
#include "vector.h"
#include <stdio.h>

void print_vector(char *, vector *);

int main(int argc, char *argv[]) {
    // create vector

    vector *v = vector_create(string_copy_constructor, string_destructor, string_default_constructor);

    vector_push_back(v, "0");    
    vector_push_back(v, "1");
    vector_push_back(v, "2");
    vector_push_back(v, "3");
    vector_push_back(v, "4");
    vector_push_back(v, "5");
    vector_resize(v, 11);
    vector_insert(v, 2, "insert");
    vector_erase(v, 0);

    vector_reserve(v, 5);


    vector_reserve(v, 17);

    vector_clear(v);


    printf("begin: %p\n", vector_begin(v));
    printf("end: %p\n", vector_end(v));
    printf("front: %p\n", vector_front(v));
    printf("back: %p\n", vector_back(v));

    vector_destroy(v);
    return 0;
}
