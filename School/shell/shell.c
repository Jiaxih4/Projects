/**
 * shell
 * CS 341 - Fall 2023
 */


#include "format.h"
#include "shell.h"
#include "vector.h"
#include <string.h>
#include "sstring.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

typedef struct process {
    char *command;
    pid_t pid;
} process;

static vector *history_vector = NULL; //vector of char*
static vector *proccess_vector = NULL; //vector of process
static FILE *source = NULL;
static int app = 0;
static int outp = 0;
static int inp = 0;
static int saved_dup = 0;

void add_process(char *comm, pid_t pid) {
    for (size_t i = 0; i < vector_size(proccess_vector); i++) {
        process *p = vector_get(proccess_vector, i);
        if (p->pid == pid) {
            p->command = comm;
            return;
        }
    }
    process *p = malloc(sizeof(process));
    p->command = comm;
    p->pid = pid;
    vector_push_back(proccess_vector, p);
}

void p_free(process* p) {
  free(p->command);
  free(p);
}

void destroy_p(pid_t pid) {
  void* v;
  int count = 0;
  while ((v = vector_get(proccess_vector, count))) {
    process *p = (process *) v;
    if (p->pid == pid ) {
      p_free(p);
      vector_erase(proccess_vector, count);
      break;
    }
    count++;
  }
}

void signal_handler(int sig) {    
  if (sig == SIGCHLD) {
    pid_t pid;
        while ((pid = waitpid((pid_t) (-1), 0, WNOHANG)) > 0) {
          destroy_p(pid);
        }
    }
}

void handleh(char* optarg) {
  FILE * f = fopen(optarg, "a");
  if (!f) {
    print_history_file_error();
    exit(1);
  }
    char *sen = NULL;
    size_t byte = 0;
    while (getline(&sen, &byte, f) != -1) {
      size_t l = strlen(sen) - 1;
      if (sen[l] == '\n') {
        sen[l] = '\0';
      }
      vector_push_back(history_vector, (void *)sen);
    }
    free(sen);
    fclose(f);
    f = NULL;
}

void handlef(char* optarg) {
  FILE *f = fopen(optarg, "r");
  if (!f) {
      print_script_file_error();
      exit(1);
  }
  source = f;
}

void theEndHasCome(){
  void* v;
  int count = 0;
  while ((v = vector_get(proccess_vector, count))) {
      process *p = (process *) v;
      kill(p->pid, SIGKILL);
      p_free(p);
      count++;
  }
  vector_destroy(proccess_vector);
}

int cd(char *comm) {
  int sparesomechange = chdir(comm+3);
  if (sparesomechange < 0) {
    print_no_directory(comm+3);
    return 1;
  } else {
    return 0;
  }
}

void pt(char** command) {
  print_command_executed(getpid());
  execvp(command[0], command);
  print_exec_failed(command[0]);
  exit(1);
}


int ex_coms(char *comm) {
  if (!strncmp(comm,"cd",2)) {
      return cd(comm);
  }  else {
    fflush(stdout);
    pid_t pid = fork();
    if (pid < 0) {
      print_fork_failed();
      exit(1);
    } else if (pid > 0) {
        process *p = calloc(1, sizeof(process));
        p->command = calloc(strlen(comm)+1, sizeof(char));
        strcpy(p->command, comm);
        p->pid = pid;
        vector_push_back(proccess_vector, p);
        if (comm[strlen(comm) - 1] == '&') {
          if (setpgid(pid, pid) == -1) {
            print_setpgid_failed();
            exit(1);
          }
        } else {
          if (setpgid(pid, getpid()) == -1) {
            print_setpgid_failed();
            exit(1);
          }
        int b;
        pid_t pr = waitpid(pid, &b, 0);
        if (pr != -1) {
          destroy_p(pr);
          if (WIFEXITED(b)){
            if (WEXITSTATUS(b)) {
              return 1;
            }
          }
        }
        else {
            print_wait_failed();
            exit(1);
        }
        }
    } else if (pid == 0) {
      if (comm[strlen(comm)-1] == '&') {
        comm[strlen(comm)-1] = '\0';
      }
      sstring *sstr = cstr_to_sstring(comm);
      vector *vec_com = sstring_split(sstr, ' ');
      free(sstr);
      size_t vs = vector_size(vec_com);
      char *comms[vs+1];
      for (size_t i = 0; i < vs; i++) {
        comms[i] = (char *) vector_get(vec_com, i);
      }
      if (!strcmp(comms[vs-1], "")) {
        comms[vs-1] = NULL;
      }
      else {
        comms[vs] = NULL;
      }
      if (app || outp || inp) {
        execvp(comms[0], comms);
        print_exec_failed(comms[0]);
        fflush(stdout);
        exit(1);
      } else {
        pt(comms);
      }
      }
    }
  return 0;
}

void pc(char* comm, ssize_t bytes_read) {
  comm[bytes_read-1] = '\0';
  if (source != stdin) {
    print_command(comm);
  }
}

void hist() {
  size_t history_size = vector_size(history_vector);
  for (size_t i = 0; i < history_size; i++) {
    print_history_line(i, (char *) vector_get(history_vector, i));
  }
}

void nth(char* comm){
  size_t num;
  size_t num_read;
  num_read = sscanf(comm+1, "%zu", &num);
  size_t history_size = vector_size(history_vector);
  if (!num_read || num > history_size-1) {
    print_invalid_index();
  } else {
    void* v = vector_get(history_vector, num);
    char *hist = (char *)v;
    print_command(hist);
    vector_push_back(history_vector, hist);
    ex_coms(hist);
  }
}

void pref(char* comm) {
  for (int i = vector_size(history_vector) - 1; i >= 0 ; i--) {
    char *his_cmd = (char *)vector_get(history_vector, i);
    if (comm[1] == '\0' || !strncmp(comm+1, his_cmd, strlen(comm+1))) {
      print_command(his_cmd);
      vector_push_back(history_vector, his_cmd);
      ex_coms(his_cmd);
      break;
    }
    if (i == 0) print_no_history_match();
  }
}

void aand(char * comm) {
  char *comm1 = strtok(comm, "&");
  comm1[strlen(comm1)-1] = '\0';
  char *comm2 = strtok(NULL, "");
  comm2 = comm2+2;
  if (!ex_coms(comm1)) {
    ex_coms(comm2);
  }
}

void oor(char* comm) {
  char *comm1 = strtok(comm, "|");
  comm1[strlen(comm1)-1] = '\0';
  char *comm2 = strtok(NULL, "");
  comm2 = comm2+2;
  if (ex_coms(comm1)) {
     ex_coms(comm2);
  }
}

void seperator(char* comm) {
  char *comm1 = strtok(comm, ";");
  char *comm2 = strtok(NULL, "");
  comm2 = comm2+1;
  ex_coms(comm1);
  ex_coms(comm2);
}

int logic(vector* parse, char* comm) {
  int op_flag = 0;
  for (size_t i = 0; i < vector_size(parse); i++) {
  char *op = (char *) vector_get(parse, i);
  if (!strcmp(op, "&&")) {
      aand(comm);
      op_flag = 1;
  } else if (!strcmp(op, "||")) {
      oor(comm);
      op_flag = 1;
  } else if (op[strlen(op)-1] == ';') {
      seperator(comm);
      op_flag = 1;
    }
  }
  return op_flag;
}

int part2(char* comm) {
      if (!strcmp(comm,"ps")) {
        return 0;
      } else if (strstr(comm, ">>") != NULL) {
        return 1;
      } else if (strstr(comm, ">") != NULL) {
        return 2;
      } else if (strstr(comm, "<") != NULL) {
        return 3;
      } else if (!strncmp(comm,"kill", 4)) {
        return 4;
      } else if (!strncmp(comm,"stop", 4)) {
        return 5;
      } else if (!strncmp(comm,"cont", 4)) {
        return 6;
      }
      return 7;
}

void choose(char* line, process_info* prcs_info) {
  char *p =  malloc(10 * sizeof(char));
  if (!strncmp(line, "State:", 6)) {
    p = strchr(line, ':');
    p++;
    while(isspace(*p)){
      p++;
    }
    prcs_info->state = *p;
  } else if (!strncmp(line, "Threads:", 8)) {
    char *c;
    p = strchr(line, ':');
    p++;
    while(isspace(*p)) ++p;
    prcs_info->nthreads = strtol(p, &c, 10);
  } else if (!strncmp(line, "VmSize:", 7)) {
    char *c;
    p = strchr(line, ':');
    p++;
    while(isspace(*p)){
      p++;
    }
    prcs_info->vsize = strtol(p, &c, 10);
  }
}

void filler1(process_info *pi, pid_t pid) {
  char *path =  malloc(50 * sizeof(char));
  char *line =  malloc(500 * sizeof(char));
  char* pd = malloc(30 * sizeof(char));
  sprintf(pd, "%d", pid);
  strcpy(path, "/proc/");
  strcat(path, pd);
  strcat(path, "/status");
  FILE *f = fopen(path,"r");
  if (!f) {
      print_script_file_error();
      exit(1);
  }
  while(fgets(line, 500, f)) {
    choose(line, pi);
  }
  fclose(f);
}

long long* timefiller(char* p, char* line) {
  long long *out = malloc(4* sizeof(long long));
  p = strtok(line, " ");
  int i = 0;
  char *end;
  while (i != 13) {
    p = strtok(NULL, " ");
    i++;
  }
  out[0] = strtol(p, &end, 10);
  p = strtok(NULL, " ");
  out[1] = strtol(p, &end, 10);
  i = 0;
  while (i != 8) {
    p = strtok(NULL, " ");
    i++;
  }
  out[2] = strtol(p, &end,10);
  return out;
}

void timestrfiller(unsigned long long utime, unsigned long long stime, process_info* pi) {
  char * c = malloc(100 * sizeof(char));
  unsigned long long add = utime + stime;
  unsigned long sec = add/sysconf(_SC_CLK_TCK);
  unsigned long long div = sec/60;
  unsigned long long q = sec%60;
  if (!execution_time_to_string(c, 100, div, q)) {
    exit(1);
  }
  pi->time_str = calloc(strlen(c)+1, sizeof(char));
  strcpy(pi->time_str, c);
}

void startfiller(unsigned long long starttime, unsigned long btime, process_info* pi) {
  char * c = malloc(100 * sizeof(char));
  time_t sec = starttime/sysconf(_SC_CLK_TCK) + btime;
  struct tm *info = localtime(&sec);
  if (!time_struct_to_string(c, 100, info)) {
    exit(1);
  }
  pi->start_str = calloc(strlen(c)+1, sizeof(char));
  strcpy(pi->start_str, c);
}

void filler2(process_info* pi, pid_t pid) {
  char *path =  malloc(50 * sizeof(char));
  char *line =  malloc(500 * sizeof(char));
  char *p =  malloc(10 * sizeof(char));
  char* pd = malloc(30 * sizeof(char));
  sprintf(pd, "%d", pid);
  strcpy(path, "/proc/");
  strcat(path, pd);
  strcat(path, "/stat");
  FILE *f = fopen(path,"r");
  if (!f) {
      print_script_file_error();
      exit(1);
  }
  fgets(line, 1000, f);
  fclose(f);
  long long * times = timefiller(p, line);
  unsigned long long utime;
  unsigned long long stime;
  unsigned long long starttime;
  unsigned long long btime;
  char* c;
  utime = times[0];
  stime = times[1];
  starttime = times[2];
  // cputime
  timestrfiller(utime, stime, pi);
  // starttime
  FILE *fs = fopen("/proc/stat","r");
  if (!fs) {
      print_script_file_error();
      exit(1);
  }
  while(fgets(line, 100, fs)) {
      if(!strncmp(line, "btime", 5)) {
        p = strchr(line, ' ');
        while(isspace(*p)) { 
          ++p;
        }
        btime = strtol(p, &c, 10);
      }
  }
  fclose(fs);
  startfiller(starttime, btime, pi);
}

process_info* create_pi(char* command, pid_t pid) {
  process_info *pi = calloc(1, sizeof(process_info));
  pi->pid = pid;
  pi->command = calloc(strlen(command)+1, sizeof(char));
  strcpy(pi->command, command);
  filler1(pi, pid);
  filler2(pi, pid);
  return pi;
}

void destroy_pi(process_info *pi) {
  free(pi->start_str);
  free(pi->time_str);
  free(pi->command);
  free(pi);
}

int findp(pid_t pid) {
  for (size_t i = 0; i < vector_size(proccess_vector); i++) {
    process *p = (process *) vector_get(proccess_vector, i);
    if (p->pid == pid) {
      p->pid = pid;
      return i;
    }
  }
  return -1;
}

void killcom(char* comm) {
  pid_t target_pid;
  sscanf(comm+4, "%d", &target_pid);
  size_t i;
  if ((i = findp(target_pid)) != -1) {
    kill(target_pid, SIGKILL);
    print_killed_process(target_pid, ((process*)vector_get(proccess_vector, i))->command);
    destroy_p(target_pid);
    return;
  }
  print_no_process_found(target_pid);
}

void stopcomm(char* comm) {
  pid_t target_pid;
  sscanf(comm+4, "%d", &target_pid);
  size_t i;
  if ((i = findp(target_pid)) != -1) {
    kill(target_pid, SIGSTOP);
    print_stopped_process(target_pid, ((process*)vector_get(proccess_vector, i))->command);
    return;
  }
  print_no_process_found(target_pid);
}

void contcomm(char* comm) {
  pid_t target_pid;
  sscanf(comm+4, "%d", &target_pid);
  size_t i;
  if ((i = findp(target_pid)) != -1) {
    kill(target_pid, SIGCONT);
    return;
  }
  print_no_process_found(target_pid);
}
char* nospace(char* c) {
  char* out = NULL;
  while (*c) {
    if (!isspace(*c)) {
      *out = *c;
      out++;
      c++;
    } else {
      c++;
    }
  }
  return out;
}
int shell(int argc, char *argv[]) {
    // TODO: This is the entry point for your shell.
    signal(SIGINT, signal_handler); //catching Ctrl + C
    signal(SIGCHLD, signal_handler); //catching exit/eof
    history_vector = string_vector_create();
    proccess_vector = shallow_vector_create();
    source = stdin;
    char* h_name = NULL;
    int opt;
    extern char* optarg;
    while ((opt = getopt(argc, argv, "h:f:")) != -1) {
      if (opt == 'h') {
          h_name = optarg;
          handleh(optarg);
      }
      else if (opt == 'f') {
          handlef(optarg);
      }
      else if (opt == '?') {
          print_usage();
          exit(1);
      }
    }
    //loop start
    char *comm = NULL;
    size_t size = 0;
    ssize_t bytes;
    //int weewoo = 0;
    char *wd_comm = malloc(256);
    char * cwd = getcwd(wd_comm, 256);
    print_prompt(cwd, getpid());
    while ((bytes = getline(&comm,&size, source)) != -1) {
      if (bytes == -1) {
        theEndHasCome();
        break;
      }
      if (bytes > 0 && comm[bytes-1] == '\n') {
        pc(comm, bytes);
      }
      //add_process(comm, getpid());
//part 2
      int p = part2(comm);
      if (p == 0) {
        print_process_info_header();
        size_t s = vector_size(proccess_vector);
        for (size_t i = 0; i < s; i++) {
            process *p = (process *) vector_get(proccess_vector, i);
            process_info *pi = create_pi(p->command, p->pid);
            print_process_info(pi);
            destroy_pi(pi);
        }
        process_info *pis = create_pi("./shell", getpid());
        print_process_info(pis);
        destroy_pi(pis);
      } else if (p == 1) {
        //>>
        app = 1;
        char* cmd = (strtok(comm, ">>"));
        if (cmd[strlen(cmd) -1] == ' ') {
          cmd[strlen(cmd) - 1] = '\0';
        }
        strtok(NULL, " ");
        char* path = (strtok(NULL, ">>"));
        int fx = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fx == -1) {
          print_script_file_error();
          exit(1);
        }
        print_command_executed(getpid());
        saved_dup = dup(1);
        dup2(fx, 1);
        ex_coms(cmd);
        dup2(saved_dup, 1);
        close(fx);
      } else if (p == 2) {
        //>
        outp = 1;
        char* cmd = (strtok(comm, ">"));
        if (cmd[strlen(cmd) -1] == ' ') {
          cmd[strlen(cmd) - 1] = '\0';
        }
        char* path = (strtok(NULL, " "));
        int fx = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fx == -1) {
          print_script_file_error();
          exit(1);
        }
        print_command_executed(getpid());
        saved_dup = dup(1);
        dup2(fx, 1);
        ex_coms(cmd);
        dup2(saved_dup, 1);
        close(fx);
      } else if (p == 3){
        //<
        char* cmd = (strtok(comm, "<"));
        if (cmd[strlen(cmd) -1] == ' ') {
          cmd[strlen(cmd) - 1] = '\0';
        }
        char* path = (strtok(NULL, " "));
        int fx = open(path, O_RDONLY);
        if (fx == -1) {
          print_script_file_error();
          exit(1);
        }
        dup2(fx, STDIN_FILENO);
        ex_coms(cmd);
        close(fx);
      } else if (p == 4) {
        killcom(comm);
      } else if (p == 5) {
        stopcomm(comm);
      } else if (p == 6) {
        contcomm(comm);
      } else
      if (!strcmp(comm,"!history")) {
          hist();
      } else if (comm[0] == '!') {
          pref(comm);
      } else if (comm[0] == '#') {
          nth(comm);
      } else if (!strcmp(comm,"exit")) {
        
          break;
      } else {
          vector_push_back(history_vector, comm);
          int op = 0;
          sstring *sstr = cstr_to_sstring(comm);
          vector *parse = sstring_split(sstr, ' ');
          op = logic(parse, comm);
          vector_destroy(parse);
          sstring_destroy(sstr);
          if (!op) {
            ex_coms(comm);
          }
        }
      wd_comm = malloc(256);
      cwd = getcwd(wd_comm, 256);
      print_prompt(cwd, getpid());
    }
    theEndHasCome();
    free(comm);
    if (h_name != NULL) {
      FILE * h = fopen(h_name, "a");
      for (size_t i = 0; i < vector_size(history_vector); i++) {
        char* car = vector_get(history_vector, i);
        fprintf(h, "%s\n", car);
      }
      free(h);
    }
    vector_destroy(history_vector);
    return 0;
}
