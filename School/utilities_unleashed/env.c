/**
 * utilities_unleashed
 * CS 341 - Fall 2023
 */
 #include <sys/types.h>
 #include <sys/wait.h>
 #include <string.h>

 #include <ctype.h>
 #include <stdio.h>
 #include <stdint.h>
 #include <unistd.h>
 #include <stdlib.h>

 #include "format.h"

int is_valid(char* arg);
int helper(char* k, char* v);
int helper2(char* k, char* v);

int is_valid(char* arg) {
    while (*arg) {
        if (!(isalnum(*arg) ||*arg == '_') ) {
            return 0;
        }
    arg++;
    }
    return 1;
}

int helper(char* k, char* v) {
    if (v == NULL) {
        return 0;
    }
    if (is_valid(k) == 0) {
        return 0;
    }
    return 39;
}

int helper2(char* k, char* v) {
    if (v[0] != '%') {
        if (is_valid(v) == 0) {
            return 0;
        }
    } else {
        v = getenv(v+1);
        if (!v) {
            return 1;
        }
    }
    if (setenv(k, v, 1) < 0) {
        return 1;
    }
    return 39;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    print_env_usage();
  }

  pid_t pt = fork();
  if (pt < 0) { 
      print_fork_failed();
  } else if (pt > 0) {
      int s;
      waitpid(pt, &s, 0);
      return 0;
  } else {
        int i = 1;
        while (argv[i]) {
          int exist = strcmp(argv[i], "--");
          if (exist) {
                char *k = strtok(argv[i], "=");
                char *v = strtok(NULL, "");
                int stats = helper(k, v);
                if (stats == 0) {
                    print_env_usage();
                }
                int stats2 = helper2(k, v);
                if (stats2 == 0) {
                    print_env_usage();
                } else if (stats2 == 1) {
                    print_environment_change_failed();
                }
          } else {
                    execvp(argv[i+1], argv+i+1);
                    print_exec_failed();
          }
          i++;
        }
        print_env_usage();
  }
  return 0;
}