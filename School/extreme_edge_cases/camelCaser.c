/**
 * extreme_edge_cases
 * CS 341 - Fall 2023
 */
#include "camelCaser.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
char **camel_caser(const char *input_str) {
    // TODO: Implement me!
    if (!input_str) {
        return NULL;
    }
    char* copy = strdup(input_str);
    int cap = 0;
    int first = 1;
    int count_s = 0;
    for (size_t i = 0; i < strlen(copy); i++) {
        char c = copy[i];
        if (ispunct(c)) {
            cap = 0;
            first = 1;
            count_s += 1;
        } else if (isspace(c)) {
            cap = 1;
        } else {
            if (isalpha(c)) {
                if (cap == 1 && first == 0) {
                    copy[i] = toupper(c);
                } else {
                    copy[i] = tolower(c);
                }
            }
            cap = 0;
            first = 0;
        }
    }
    char** out = malloc((count_s + 1) * sizeof(char*));
    out[count_s] = NULL;
    int count_s_counter = 0;
    int count_c = 0;
    for (size_t i = 0; i < strlen(copy); i++) {
        if (count_s_counter < count_s) {
        char c = copy[i];
        if (ispunct(c)) {
            out[count_s_counter] = malloc((count_c + 1) * sizeof(char));
            out[count_s_counter][count_c] = '\0';
            count_s_counter += 1;
            count_c = 0;
        } else if (!isspace(c)){
            count_c += 1;
        }
        }
    }
    count_s_counter = 0;
    count_c = 0;
    for (size_t i = 0; i < strlen(copy); i++) {
        if (count_s_counter < count_s) {
        if(!copy[i]) {
            break;
        }
        char c = copy[i];
        if (ispunct(c)) {
            count_s_counter += 1;
            count_c = 0;
        } else {
            if (!isspace(c)) {
                out[count_s_counter][count_c] = c;
                count_c += 1;
            }
        }
        }
    }
    free(copy);
    copy = NULL;
    return out;

}

void destroy(char **result) {
    // TODO: Implement me!
    char ** s;
    for (s = result; *s != NULL; s++) {
        free(*s);
        *s = NULL;
    }
    free(result);
    result = NULL;
}
