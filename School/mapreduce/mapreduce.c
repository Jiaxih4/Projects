/**
 * mapreduce
 * CS 341 - Fall 2023
 */
#include "utils.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void wait_finish(int count, pid_t* p) {
    for (int i = 0; i < count;i++) {
      int status;
      waitpid(p[i], &status, 0);
    }
}
int main(int argc, char **argv) {
    if (argc != 6) {
      print_usage();
      return 1;
    }
    char *input = argv[1];
    char *output = argv[2];
    char *mapper = argv[3];
    char *reducer = argv[4];
    char *num = argv[5];
    int count = atoi(num);
    // Create an input pipe for each mapper.
    int maps[count * 2];
    for (int i = 0; i < count * 2;i+= 2) {
      pipe(maps + i);
    }

    // Create one input pipe for the reducer.
    int reds[2];
    pipe(reds);

    // Open the output file.
    int op = open(output,  O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    // Start a splitter process for each mapper.
    pid_t sp[count];
    for (int i = 0; i < count;i++) {
      sp[i] = fork();
      if (sp[i] == 0) {
        char s[10];
        sprintf(s, "%d", i);
        close(maps[i*2]);
        dup2(maps[i*2+1], 1);
        execl("./splitter", "./splitter", input, argv[5], s, NULL);
        exit(1);
      }
    }

    // Start all the mapper processes.
    pid_t mp[count];

    for (int i = 0; i < count;i++) {
        close(maps[i * 2 + 1]);
        mp[i] = fork();
        if (mp[i] == -1) {
            exit(1);
        } else if (mp[i]== 0) {
            int d0 = dup2(maps[i * 2], 0);
            int d1 = dup2(reds[1], 1);
            if (d1 == -1 || d0 == -1 || execl(mapper, mapper, NULL) == -1) {
                exit(1);
            }
        }
    }

    // Start the reducer process.
    close(reds[1]);
    pid_t f = fork();
    if (f == 0) {
      dup2(reds[0], 0);
      dup2(op, 1);
      execl(reducer, reducer, NULL);
      exit(1);
    }
    close(reds[0]);
    close(op);

    // Wait for the reducer to finish.
    wait_finish(count, sp);
    for (int i = 0; i < count;i++) {
        close(maps[i*2]);
    }
    wait_finish(count, mp);
    int status;
    waitpid(f, &status, 0);


    // Print nonzero subprocess exit codes.
    if (status)
      print_nonzero_exit_status(reducer, status);
    // Count the number of lines in the output file.
    print_num_lines(output);
    return 0;
}
