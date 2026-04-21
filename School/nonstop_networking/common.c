/**
 * nonstop_networking
 * CS 341 - Fall 2023
 */
#include "common.h"

ssize_t read_socket(int socket, char *buffer, size_t count) {
    size_t rd = 0;
    int repeat = 1;
    while (1) {
      if (rd >= count) {
        break;
      }
      ssize_t rc = read(socket, buffer + rd, count - rd);
      if (rc == 0) {
        break;
      }
      if (rc == -1 && errno == EINTR) {
        continue;
      } else {
        if (rc == -1 && errno != EINTR) {
        return -1;
        }
        rd += rc;
        repeat++;
      }
    }
    return rd;
}

ssize_t write_socket(int socket, const char *buffer, size_t count) {
    size_t wr = 0;
    int repeat = 1;
    while (1) {
      if (wr >= count) {
        break;
      }
      ssize_t ret = write(socket, buffer + wr, count - wr);
      if (ret == 0) {
        break;
      }
      if (ret == -1 && errno == EINTR) {
        continue;
      } else {
        if (ret == -1) {
          return -1;
        }
        wr += ret;
        repeat++;
      }
    }
    return wr;
}

size_t s_filler(int socket) {
  size_t size;
  read_socket(socket, (char *)&size, sizeof(size_t));
  return size;
}

size_t br(size_t size, int socket, size_t s, int lev, FILE* f){
  char buffer_f[size];
  memset(buffer_f, 0, size);
  size_t rc = read_socket(socket, buffer_f, s);
  if (lev == 1) {
    fwrite(buffer_f, 1, rc, f);
  } else if (lev == 0) {
    fprintf(stdout, "%zu%s", size - 6, buffer_f);
  }
  return rc;
}

int g_handler(char** args, int socket) {
  FILE *f = fopen(args[4], "a+");
  if (!f) {
    return 1;
  }
  size_t size;
  size = s_filler(socket);
  size_t rd = 0;
  size_t s;
  while (rd < size+5) {
    s = size + 5 - rd;
    if (s > LIMIT){
      s = LIMIT;
    }
    size_t rc = br(LIMIT+1, socket, s, 1, f);
    rd += rc;
    if (rc == 0) {
      break;
    }
  }
  if (rd == 0 && 0 != size) {
    print_connection_closed();
    return 1;
  } else if (rd < size) {
    print_too_little_data();
    return 1;
  } else if (rd > size) {
    print_received_too_much_data();
    return 1;
  }
  fclose(f);
  return 0;
}

int l_handler(int socket, size_t rd) {
  size_t size;
  size = s_filler(socket);
  size += 6;
  rd = br(size, socket, size -1, 0, NULL);
  if (rd == 0 && 0 != size - 6) {
    print_connection_closed();
    return 1;
  } else if (rd < size -6) {
    print_too_little_data();
    return 1;
  } else if (rd > size -6) {
    print_received_too_much_data();
    return 1;
  }
  return 0;
}

int success(char* buffer, size_t rd, verb method, char** args, int socket) {
  fprintf(stdout, "%s", buffer);
  if (method == DELETE || method == PUT) {
    print_success();
  } else if (method == GET) {
    int lev = g_handler(args, socket);
    if (lev == 1) {
      return lev;
    }
  } else if (method == LIST) {
    int lev = l_handler(socket, rd);
    if (lev == 1) {
      return 1;
    }
  }
  return 0;
}

void error_exist(char* buffer, int socket) {
  fprintf(stdout, "%s", buffer);
  char err[39];
  memset(err, 0, 39);
  if (read_socket(socket, err, 39) == 0) {
    print_connection_closed();
  }
  print_error_message(err);
}

void failure(char* buffer, size_t rd, int socket) {
  size_t el = strlen("ERROR") + 1;
  buffer = realloc(buffer, el+1);
  size_t d = el - rd;
  read_socket(socket, &(buffer[rd]), d);
  if (strcmp(buffer, "ERROR\n") == 0) {
    error_exist(buffer, socket);
  } else {
    print_invalid_response();
  }
}

void read_server(char **args, int socket, verb method) {
  size_t kl = strlen("OK") + 1;
  char *buffer = calloc(1,kl+1);
  size_t rd = read_socket(socket, buffer, kl);
  if (strcmp(buffer, "OK\n") == 0) {
    int lev = success(buffer, rd, method, args, socket);
    if (lev == 1) {
      exit(1);
    }
  } else {
    failure(buffer, rd, socket);
  }
  free(buffer);
}

int l_filler(char** args, verb method) {
  int l = 0;
  if (method == LIST) {
    l = strlen(args[2]) + strlen("\n") + 1;
  } else {
    l = strlen(args[2]) + strlen(args[3]) + strlen("\n") + strlen("\n") + 1;
  }
  return l;
}



int write_header(char ** args, int socket, verb method){
  int l = l_filler(args, method);
  char str[l];
  if (method == LIST) {
    strcpy(str, args[2]);
    strcat(str, "\n");
  } else {
    strcpy(str, args[2]);
    strcat(str, " ");
    strcat(str, args[3]);
    strcat(str, "\n");
  }
  ssize_t len = strlen(str);
  if (write_socket(socket, str, len) < len) {
    print_connection_closed();
    return 1;
  }
  return 0;
}

size_t seek_size(FILE* f) {
  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  fseek(f, 0, SEEK_SET);
  return size;
}

int p_write_all(size_t tt, size_t size, int socket, FILE* f) {
  while (tt < size) {
    ssize_t ss = size - tt;
    if (ss > LIMIT ){
        ss = LIMIT;
    }
    char buffer[ss + 1];
    fread(buffer, 1, ss, f);

    size_t rc = write_socket(socket, buffer, ss);
    if ((ssize_t)rc < ss) {
      print_connection_closed();
      return -1;
    }
    tt += rc;
  }
  return 0;
}
int p_handler(char** args, int socket) {
  FILE *f = fopen(args[4], "r");
  if (!f) {
    return 1;
  }
  size_t size = seek_size(f);
  write_socket(socket, (char*) &size, sizeof (size_t));  

  size_t tt = 0;
  int lev = p_write_all(tt, size, socket, f);
  if (lev == -1) {
    return -1;
  }
  fclose(f);
  return 0;
}

int write_body(char** args, int socket, verb method) {
  if (method == PUT) {
    int lev = p_handler(args, socket);
    if (lev == 1) {
      return lev;
    } else if (lev == -1) {
      exit(-1);
    }
  }
  return 0;
}

void write_to_server(char **args, int socket, verb method) {
  int lev = write_header(args, socket, method);
  if (lev == 1) {
    exit(1);
  }
  int lev2 = write_body(args, socket, method);
  if (lev2 == 1) {
    exit(1);
  }
}
int connection(char **args) {
  int sk = socket(AF_INET, SOCK_STREAM, 0);
  if (sk == -1) {
      exit(1);
  }
  struct addrinfo hints;
  struct addrinfo* result;
  size_t as = sizeof(struct addrinfo);
  memset(&hints, 0, as);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  int s = getaddrinfo(args[0], args[1], &hints, &result);
  if (s != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
      exit(1);
  }
  if (connect(sk, result->ai_addr, result->ai_addrlen) == -1) {
      exit(1);
  }
  freeaddrinfo(result);
  return sk;
}

void first_part(char** args, int sk, verb method){
    write_to_server(args, sk, method);
    if (shutdown(sk, SHUT_WR) != 0) {
        perror("shutdown()");
    }
}

void second_part(char** args, int sk, verb method) {
  read_server(args, sk, method);
  shutdown(sk, SHUT_RD);
}
