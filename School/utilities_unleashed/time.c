/**
 * utilities_unleashed
 * CS 341 - Fall 2023
 */

#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "format.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_time_usage();
    }

    pid_t pt = fork();

    if (pt <= -1) {
        print_fork_failed();
    }
    if (pt == 0) {
        int ex = execvp(argv[1], argv + 1);
        if (ex) {
            print_exec_failed();
        }
    }
    else if (pt > 0){
        struct timespec begin, end;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        int s;

        waitpid(pt, &s, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (WIFEXITED(s) && WEXITSTATUS(s) == 0) {
            display_results(argv, end.tv_sec - begin.tv_sec + (end.tv_nsec - begin.tv_nsec)/1000000000.0);
        }
    }

    return 0;
}