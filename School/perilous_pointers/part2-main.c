/**
 * perilous_pointers
 * CS 341 - Fall 2023
 */
#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
    // your code here
    
    first_step(81);
    
    int i = 132;
    second_step(&i);
    
    i = 8942;
    int *x = & i;
    int **p = &x;
    double_step(p);
    
    char *c = malloc(6 + sizeof(int));
    *(int*)(c+5) = 15;
    strange_step(c);
    free(c);
    c = NULL;
    
    char *a = malloc(5*sizeof(char));
    a[3] = 0;
    empty_step((void*)a);
    free(a);
    a = NULL;
    
    char *b = malloc(4*sizeof(char));
    b[3] = 'u';
    two_step((void*)b,b);
    free(b);
    b = NULL;
    
    char *d = malloc(6 * sizeof(char));
    char *d2 = d+2;
    char *d4 = d+4;
    three_step(d, d2, d4);
    free(d);
    d = NULL;
    
    char *e = malloc(4*sizeof(char));
    for (int k = 0; k < 4; k++) {
        e[k] = k*8;
    }
    step_step_step(e,e,e);
    free(e);
    e = NULL;
    
    char g = 39;
    it_may_be_odd(&g, (int)g);
    
    char h[] = " ,CS241";
    tok_step(h);

    char* orange = malloc( 2 * sizeof(char));
    orange[0] = 1;
    orange[1] = 2;
    the_end((void*)orange, (void*)orange);
    free(orange);
    orange = NULL;


    return 0;

}
