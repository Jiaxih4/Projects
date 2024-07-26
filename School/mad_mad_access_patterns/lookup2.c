/**
 * mad_mad_access_patterns
 * CS 341 - Fall 2023
 */
#include "tree.h"
#include "utils.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses mmap to access the data.

  ./lookup2 <data_file> <word> [<word> ...]
*/

void lookup2(char* word, int offset, void* data) {
  if (offset == 0) {
    printNotFound(word);
    return;
  }
  BinaryTreeNode * n = (BinaryTreeNode*) (data + offset);
  if (strcmp(n->word, word) == 0) {
    printFound(word, n->count, n->price);
  } else if (strcmp(n->word, word) > 0) {
    lookup2(word, n->left_child, data);
  } else {
    lookup2(word, n->right_child, data);
  }
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printArgumentUsage();
    exit(1);
  }
  char* df =argv[1];
  int f = open(df, O_RDONLY);
  if (f == -1) {
    openFail(df);
    exit(2);
  }
  struct stat st;
  if (fstat(f, &st) != 0) {
    openFail(df);
    exit(2);
  }
  void* data = mmap(0, st.st_size, PROT_READ, MAP_SHARED, f, 0);
  if (data == MAP_FAILED) {
    mmapFail(df);
    exit(3);
  }
  if (strncmp(data, BINTREE_HEADER_STRING, BINTREE_ROOT_NODE_OFFSET) != 0) {
    formatFail(df);
    exit(2);
  }
  int i = 2;
  while (i < argc) {
    lookup2(argv[i], BINTREE_ROOT_NODE_OFFSET, data);
    i++;
  }
  close(f);
    return 0;
}
