#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

void my_malloc(size_t size);
int my_free(void *ptr);
Block *find_free_block(Block *block, size_t size);
Block *request_space(Block *block, size_t size);

// Manage every block of memory in the heap
typedef struct Block {
  size_t size;
  int free;
  struct Block *next;
}
Block;

#define BLOCK_SIZE sizeof(Block)

Block* free_list = NULL;

int main() {
  printf("Memory allocator");

  return 0;
}

void my_malloc(size_t size) {
  void *ptr;
  // increase size of heap with sbrk and retrieve memory
  // check if memory is free
  // if memory is free
  //    allocate memory
  // else
  //    ask for other memory ?
}

Block *find_free_block(Block **last, size_t size) {
  Block *current = free_list;
  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }

  return current;
}

Block *request_space(Block *block, size_t size) {
  
}
