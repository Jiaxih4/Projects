/**
 * nonstop_networking
 * CS 341 - Fall 2023
 */
#pragma once
#include <stddef.h>
#include <sys/types.h>
#include "format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define LIMIT 1024

#define LOG(...)                      \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n");        \
    } while (0);

typedef enum { GET, PUT, DELETE, LIST, V_UNKNOWN } verb;

ssize_t write_socket(int socket, const char *buffer, size_t count);
ssize_t read_socket(int socket, char *buffer, size_t count);
size_t s_filler(int socket);
size_t br(size_t size, int socket, size_t size_hd, int lev, FILE* local);
int g_handler(char** args, int socket);
int l_handler(int socket, size_t bytes_rd);
int success(char* buffer, size_t bytes_rd, verb method, char** args, int socket);
void error_exist(char* buffer, int socket);
void failure(char* buffer, size_t bytes_rd, int socket);
void read_server(char **args, int socket, verb method);
int write_header(char ** args, int socket, verb method);
size_t seek_size(FILE* local);
int p_write_all(size_t wtotal, size_t size, int socket, FILE* local);
int p_handler(char** args, int socket);
int write_body(char** args, int socket, verb method);
void write_to_server(char **args, int socket, verb method);
int connection(char **args);
void first_part(char** args, int serverSocket, verb method);
void second_part(char** args, int sk, verb method);
