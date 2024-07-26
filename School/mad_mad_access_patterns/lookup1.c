/**
 * mad_mad_access_patterns
 * CS 341 - Fall 2023
 */
#include "tree.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses fseek() and fread() to access the data.

  ./lookup1 <data_file> <word> [<word> ...]
*/

void lookup(int offset, char* word, FILE* f, char* df);

int main(int argc, char **argv) {
  if (argc < 3) {
    printArgumentUsage();
    exit(1);
  }
  char* df =argv[1];
  FILE* f = fopen(df, "r");
  if (f == NULL) {
    openFail(df);
    exit(2);
  }
  char base[BINTREE_ROOT_NODE_OFFSET];
  fread(base, 1, BINTREE_ROOT_NODE_OFFSET, f);
  if (strcmp(base, BINTREE_HEADER_STRING) != 0) {
    formatFail(df);
    exit(2);
  } 
  for (int i = 2; i < argc; i++) {
    lookup(BINTREE_ROOT_NODE_OFFSET, argv[i], f, df);
  }
    return 0;
}

void seeker(int offset, FILE* f) {
  if (fseek(f, offset, SEEK_SET) == -1) {
    exit(2);
  }
}

void lookup(int offset, char* word, FILE* f, char* df) {
  if (offset == 0) {
    printNotFound(word);
    return;
  }
  seeker(offset, f);

  BinaryTreeNode root;
  if (fread(&root, sizeof(BinaryTreeNode), 1, f) < 1) {
    exit(2);
  }

  seeker(offset + sizeof(BinaryTreeNode), f);

  char n[100];
  if (fgets(n, 100, f) == NULL) {
    formatFail( df);
    exit(2);
  }
  if (strcmp(n, word) == 0) {
    printFound(word, root.count, root.price);
    return;
  } else if (strcmp(n, word) > 0) {
    lookup(root.left_child, word, f, df);
  } else {
    lookup(root.right_child, word, f, df);
  }

}