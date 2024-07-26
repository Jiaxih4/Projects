/**
 * charming_chatroom
 * CS 341 - Fall 2023
 */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "utils.h"
static const size_t MESSAGE_SIZE_DIGITS = 4;

char *create_message(char *name, char *message) {
    int name_len = strlen(name);
    int msg_len = strlen(message);
    char *msg = calloc(1, msg_len + name_len + 4);
    sprintf(msg, "%s: %s", name, message);

    return msg;
}

ssize_t get_message_size(int socket) {
    int32_t size;
    ssize_t read_bytes =
        read_all_from_socket(socket, (char *)&size, MESSAGE_SIZE_DIGITS);
    if (read_bytes == 0 || read_bytes == -1)
        return read_bytes;

    return (ssize_t)ntohl(size);
}

// You may assume size won't be larger than a 4 byte integer
ssize_t write_message_size(size_t size, int socket) {
    // Your code here
    size_t nsize = htonl(size);
    ssize_t output = write_all_to_socket(socket, (char*) &nsize, MESSAGE_SIZE_DIGITS);
    return output;
}

ssize_t read_all_from_socket(int socket, char *buffer, size_t count) {
    // Your Code Here
    size_t output = 0;
    while (output < count) {
        ssize_t r = read(socket, buffer + output, count - output);
        if (r == 0) {
            break;
        } else if (r == -1) {
            if (errno != EINTR) {
                return -1;
            }
        }
        output += r;
    }
    return (ssize_t)output;
}

ssize_t write_all_to_socket(int socket, const char *buffer, size_t count) {
    // Your Code Here
    size_t output = 0;
    while (output < count) {
        ssize_t w = write(socket, buffer + output, count - output);
        if (w > 0) {
            output += (size_t)w;
        } else if (w == 0) {
            return 0;
        } else if(w == -1) {
            if (errno != EINTR ) {
                return -1;
            }
        }
    }
    return (ssize_t)output;
}
