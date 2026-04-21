/**
 * extreme_edge_cases
 * CS 341 - Fall 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camelCaser.h"
#include "camelCaser_tests.h"

int test_camelCaser(char **(*camelCaser)(const char *),
                    void (*destroy)(char **)) {
    // TODO: Implement me!

    
    char **outputs = 0;
    outputs = (*camelCaser)("");
    if (outputs[0] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }
    outputs = (*camelCaser)("oh howdy world.");
    if (strcmp(outputs[0], "ohHowdyWorld")) {
        destroy(outputs);
        return 0;
    }
    if (outputs[1] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }

    outputs = (*camelCaser)("Oh Howdy World");
    if (outputs[0] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }
    outputs = (*camelCaser)("Oh Howdy World.");
    if (strcmp(outputs[0], "ohHowdyWorld")) {
        destroy(outputs);
        return 0;
    }
    if (outputs[1] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }

    outputs = (*camelCaser)("oH hOWDy wORLd.");
    if (strcmp(outputs[0], "ohHowdyWorld")) {
        destroy(outputs);
        return 0;
    }
    if (outputs[1] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }
    outputs = (*camelCaser)("?&^!@.");
    for (int i = 0; i < 6; i++) {
        if (strcmp(outputs[i], "")) {
            destroy(outputs);
            return 0;
        }
    }
    if (outputs[6] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }

    outputs = (*camelCaser)("Oh. Howdy. World.");
    if (strcmp(outputs[0], "oh")) {
        destroy(outputs);
        return 0;
    }
    if (strcmp(outputs[1], "howdy")) {
        destroy(outputs);
        return 0;
    }
    if (strcmp(outputs[2], "world")) {
        destroy(outputs);
        return 0;
    }
    if (outputs[3] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }

    outputs = (*camelCaser)("1oh. 1howdy. 2world.");
    if (strcmp(outputs[0], "1oh")) {
        destroy(outputs);
        return 0;
    }
    if (strcmp(outputs[1], "1howdy")) {
        destroy(outputs);
        return 0;
    }
    if (strcmp(outputs[2], "2world")) {
        destroy(outputs);
        return 0;
    }
    if (outputs[3] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }
    outputs = (*camelCaser)("oh.                          Howdy               . World            .");
    if (strcmp(outputs[0], "oh")) {
        destroy(outputs);
        return 0;
    }
    if (strcmp(outputs[1], "howdy")) {
        destroy(outputs);
        return 0;
    }
    if (strcmp(outputs[2], "world")) {
        destroy(outputs);
        return 0;
    }
    if (outputs[3] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }
        outputs = (*camelCaser)("*oh Howdy world?");
    if (strcmp(outputs[0], "")) {
        destroy(outputs);
        return 0;
    }
    if (strcmp(outputs[1], "ohHowdyWorld")) {
        destroy(outputs);
        return 0;
    }
    if (outputs[2] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }

    outputs = (*camelCaser)("oh\x3Howdy\x9fWorld\x11.");
    if (strcmp(outputs[0], "oh\3howdy\x9fworld\x11")) {
        destroy(outputs);
        return 0;
    }
    if (outputs[1] != NULL) {
        destroy(outputs);
        return 0;
    }
    destroy(outputs);
    outputs = (*camelCaser)("OH HOWDY WORLD.");
    if (strcmp(outputs[0], "ohHowdyWorld")) {
        destroy(outputs);
        return 0;
    }
    if (outputs[1] != NULL) {
        destroy(outputs);
        return 0;
    } else {
        destroy(outputs);
    }

    return 1;
}
