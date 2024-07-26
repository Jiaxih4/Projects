/**
 * mini_memcheck
 * CS 341 - Fall 2023
 */
#include "mini_memcheck.h"
#include <stdio.h>
#include <string.h>

meta_data* head;
size_t total_memory_requested = 0;
size_t total_memory_freed = 0;
size_t invalid_addresses = 0;

void changeMem(size_t old_request_size, size_t request_size) {
  if (old_request_size < request_size) {
      total_memory_requested += request_size - old_request_size;
  } else if (old_request_size > request_size) {
      total_memory_freed += old_request_size - request_size;
  }
}

void move(meta_data* prev, meta_data* curr, void* payload) {
    while (curr) {
      void* memory_payload = (void*)curr + sizeof(meta_data);
      if (memory_payload == payload) {
        break;
      }
      prev = curr;
      curr = curr->next;
    }
}

void fill(meta_data* meta, size_t request_size, const char *filename, void *instruction) {
    meta->request_size = request_size;
    meta->filename = filename;
    meta->instruction = instruction;
}

void insert(meta_data* meta, size_t request_size, const char *filename, void *instruction) {
    fill(meta, request_size, filename, instruction);
    meta->next = head;
    head = meta;

}
void *mini_malloc(size_t request_size, const char *filename,
                  void *instruction) {
    // your code here
    if (request_size == 0) {
        return NULL;
    }
    void* payload = malloc(sizeof(meta_data) + request_size);
    if (!payload) {
        return NULL;
    }

    meta_data* meta = (meta_data*) payload;
    insert(meta, request_size, filename, instruction);
    total_memory_requested = total_memory_requested + request_size;
    return payload + sizeof(meta_data);
}

void *mini_calloc(size_t num_elements, size_t element_size,
                  const char *filename, void *instruction) {
    // your code here
    if (num_elements == 0) {
      return NULL;
    }
    if (element_size == 0) {
        return NULL;
    }

    size_t request_size = num_elements * element_size;
    void* payload = mini_malloc(request_size, filename, instruction);
    if (!payload) {
         return NULL;
    }

    memset(payload, 0, request_size);
    return payload;
}

void *mini_realloc(void *payload, size_t request_size, const char *filename,
                   void *instruction) {
    // your code here
    if (!payload) {
        return mini_malloc(request_size, filename, instruction);
    }
    if (request_size == 0) {
      mini_free(payload);
      return NULL;
    }

    meta_data* prev = NULL;
    meta_data* curr = head;
    move(prev, curr, payload);

    if (!curr) {
      invalid_addresses++;
      return NULL;
    }

    size_t old_request_size = curr->request_size;
    changeMem(old_request_size, request_size);

    meta_data* nm = (meta_data*) realloc(curr, request_size + sizeof(meta_data));
    if (!nm) return NULL;

    fill(nm, request_size, filename, instruction);

    if (nm != curr) {
      prev->next = nm;
      nm->next = curr->next;
      free(curr);
    }
    return nm + 1;
}

void mini_free(void *payload) {
    // your code here
    if (!payload) {
      return;
    }
    meta_data *prev = NULL;
    meta_data *curr = head;
    while (curr) {
        void *memory = ( (void *) curr ) + sizeof(meta_data);
        if (payload == memory) {
            if (prev) {
                prev->next = curr->next;
            } else {
                head = curr->next;
            }
            total_memory_freed += curr->request_size;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    invalid_addresses++;
}
