/**
 * mini_memcheck
 * CS 341 - Fall 2023
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Your tests here using malloc and free
    void *p1 = malloc(30);
    p1 = realloc(p1, 500);
    free(p1);
    return 0;
}