/**
 * malloc
 * CS 341 - Fall 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>

typedef struct mdata {
  struct mdata *prev;
	struct mdata *next;
	int free;
	size_t size;
} mdata;

static mdata* head[16];
static size_t ms = sizeof(mdata);
int find_indx(size_t);
void remov(mdata*, int);
void removenb(mdata* new_mdata);
void removecurr(mdata* curr, int idx);
int compforrem(mdata* curr);
void de(int idx);
mdata* bfit(mdata* curr, mdata* best_fit, size_t size);
mdata* gothroughcurronceagain(mdata* best_fit, size_t size);
mdata* gothroughcurrforthirdtime(mdata* best_fit, size_t size, mdata* curr);
mdata* fill_block(size_t size, mdata* block);
void fillmd(mdata* new_block, size_t new_size, int new_free, mdata* new_next, mdata* new_prev);
int returnbird(mdata* free_this, size_t ms, void* ptr);
void free99(mdata* free_this, int idx);
void freebird(mdata* free_this, size_t ms, void* ptr);


/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
	size_t mul = num * size;
  void *output = malloc(mul);
  if (output == NULL) {
    return output;
  }
  memset(output, 0, mul);
  return output;
}

/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */

void *malloc(size_t size) {
	if (size != 0) {
	  int idx = find_indx(size);
	  de(idx);
	  mdata *block = NULL;
	  if (head[idx] != NULL) {
      block = fill_block(size, block);
		  if (block != NULL) {
			  remov(block, idx);
			  block->free = 0;
			  return (void *)(block + 1);
		  }
	  }
    mdata* new_block = sbrk(ms + size);
	  if (new_block == (void*) -1) {
		  return NULL;
	  }
    fillmd(new_block, size, 0, NULL, NULL);
	  if (new_block != NULL) {
		  return (void *)(new_block + 1);
    }
  }
	return NULL;
}

/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */

void free(void *ptr) {
	if (!ptr) {
		return;
	}
	mdata* free_this = (mdata *)ptr - 1;
	assert(free_this->free == 0);
	freebird( free_this, ms, ptr);
}

/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
	if (!ptr) {
    return malloc(size);
  }
  mdata* meta = (void *)ptr - ms;
  if (meta->size >= size) {
    return ptr;
  }
  void *mem = malloc(size);
  if (mem == NULL) {
    return NULL;
  }
  memcpy(mem, ptr, meta->size);
  free(ptr);
  return mem;
}

mdata* gothroughcurrforthirdtime(mdata* best_fit, size_t size, mdata* curr){
	while (curr != NULL) {
		if (size <= curr->size) {
			best_fit = curr;
      break;
		}
		curr = curr->next;
	  }
  return best_fit;
}

mdata* fill_block(size_t size, mdata* block) {
  int idx = find_indx(size);
  if (find_indx(size) > 4) {
    mdata* best_fit = NULL;
	  best_fit = gothroughcurronceagain(best_fit, size);
    block = best_fit;
  } else {
    mdata* curr = head[idx];
    mdata* best_fit = NULL;
	  best_fit = gothroughcurrforthirdtime(best_fit, size, curr);
    block = best_fit;
  }
  return block;
}

void removenb(mdata* new_mdata) {
  int next_idx = find_indx(new_mdata->size);
	remov(new_mdata, next_idx);
}
void removecurr(mdata* curr, int idx) {
  remov(curr, idx);
	sbrk(0 - (curr->size + ms));
}

 
int l(size_t div) {
  return (int)ceil(log(div)/log(2));
}

int try_log_base_2(size_t size) {
  if (size < 0) {
    return 15;
  }
	size_t div = size /8;
  size_t rem = size % 8;
  int out = l(div);
  if (rem != 0) {
    out++;
  }
  if (out >= 0 && out < 15) {
    return out;
  }
	return 15;
}

int returnbird(mdata* free_this, size_t ms, void* ptr) {
  void* p = ptr + free_this->size;
  if (find_indx(free_this->size) > 4 && (p >= sbrk(0))) {
		sbrk(0 - (free_this->size + ms));
		return 1;
	}
  return 0;
}
void free99(mdata* free_this, int idx) {
	free_this->next = head[idx];
  free_this->prev = NULL;
	if (head[idx] != NULL){
		head[idx]->prev = free_this;
  }
	head[idx] = free_this;
}

mdata* bfit(mdata* curr, mdata* best_fit, size_t size) {
  int comp = ((!best_fit) || (best_fit->size > curr->size));
  if (comp && curr->size >= size) {
		best_fit = curr;
	}
  return best_fit;
}

mdata* gothroughcurronceagain(mdata* best_fit, size_t size) {
  int idx = find_indx(size);
  mdata* curr = head[idx];
  while (curr != NULL) {
		best_fit = bfit(curr, best_fit, size);
    curr = curr->next;
	}
  return best_fit;
}

int find_indx(size_t size) {
	for (int i = 0; i < 15; ++i) {
    size_t comp = pow(2, i);
    comp *= 8;
		if (size <= comp) {
			return i;
		}
	}
	return 15;
}

int compforrem(mdata* curr) {
  size_t s = curr->size;
  void* c = (void*) curr;
  curr += ms;
  curr += s;
  void* v =  c;
  if ((v) >= sbrk(0)) {
    return 1;
  } else {
    return 0;
  }
}

void de(int idx) {
  if (idx == 15) {
		mdata *curr = head[idx];
	  while (curr != NULL) {
      char* c = (char*) curr;
      size_t cs = curr->size;
      c += ms;
      c += cs;
		  if (((void*)c) >= sbrk(0)) {
			  removecurr(curr, idx);
			  return;
		  }
		  curr = curr->next;
	  }
	}
}

void fillmd(mdata* new_block, size_t new_size, int new_free, mdata* new_next, mdata* new_prev) {
  new_block->free = new_free;
  new_block->size = new_size;
  new_block->next = new_next;
  new_block->prev = new_prev;
}

void removHelper(mdata* prev, mdata* next, int idx) {
  if (prev == NULL && next == NULL) {
		head[idx] = NULL;
	} else if (prev == NULL) {
		next->prev = NULL;
		head[idx] = next;
	} else if (next == NULL) {
		prev->next = NULL;
	} else {
		prev->next = next;
		next->prev = prev;
	}
}
void remov(mdata* node, int idx) {
  mdata* next = node->next;
	mdata* prev = node->prev;
	removHelper(prev, next, idx);
	node->next = NULL;
	node->prev = NULL;
}

void freebird(mdata* free_this, size_t ms, void* ptr) {
	if (returnbird(free_this,ms,ptr) == 1) {
    return;
  } else {}
	free_this->free = 1;
	if (free_this != NULL) {
	  int idx = find_indx(free_this->size);
	  free99(free_this, idx);
  }
}