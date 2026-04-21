/**
 * finding_filesystems
 * CS 341 - Fall 2023
 */
#include "minixfs.h"
#include "minixfs_utils.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Write tests here!
    file_system* fs = open_fs("test.fs");
    char *buffer = calloc(1, 1020);
    off_t off = 16384;
    char buf[20];
    minixfs_write(fs, "non.txt", buf, strlen(buf), &off);
    char expected[20];
    printf("off:%ld\n", off);
    assert(!strcmp(buf, expected));
    free(buffer);
    return 0;
}
