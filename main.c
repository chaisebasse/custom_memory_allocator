#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Block {
  size_t size;
  int free;
  struct Block *next;
  struct Block * prev;
} Block;

void *my_malloc(size_t size);

Block *find_free_block(Block **last, size_t size);
Block *request_space(Block *last, size_t size);
void my_free(void *ptr);
#define BLOCK_SIZE sizeof(Block)

Block* free_list = NULL;

void print_block_info() {
  Block *block = free_list;
  while (block) {
    printf("size: %zu, free: %s, next: %p, prev: %p\n", block->size, block->free ? "true" : "false", block->next, block->prev);
    block = block->next;
  }
  printf("\n");
}

int main() {
  int* arr = (int*) my_malloc(10 * sizeof(int));
  int* arr1 = (int*) my_malloc(5 * sizeof(int));
  int* arr2 = (int*) my_malloc(8 * sizeof(int));
  if (arr == NULL || arr1 == NULL || arr2 == NULL) {
    fprintf(stderr, "my_malloc failed\n");
    return 1;
  }

  for (int i = 0; i < 10; i++) {
    arr[i] = i;
    printf("%d ", arr[i]);
  }
  printf("\n");


  Block *block = free_list;
  Block *block2 = free_list;
  do
  {
    printf("size : %i, free : %s, next : %p, prev : %p\n", (int)block->size, block->free ? "true" : "false", block->next, block->prev);

    block = block2;
    block2 = block2->next;
  } while (block2);

  my_free(arr);
  my_free(arr1);
  my_free(arr2);
  return 0;
}

Block *find_free_block(Block **last, size_t size) {
  Block *current = free_list;
  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }

  return current;
}

Block *request_space(Block *last, size_t size) {
  Block *block;
  block = sbrk(0);

  void *request = sbrk(size + BLOCK_SIZE);
  if (request ==  (void *)-1) {
    perror("sbrk fail (request_space)");
    return NULL;
  }

  block->size = size;
  block->next = NULL;
  block->free = false;
  block->prev = last;

  if (last) {
    last->next = block;
  }

  return block;
}

void *my_malloc(size_t size) {
  if (size <= 0) {
    perror("requested size too small (my_malloc)");
    return NULL;
  }

  Block *block;

  if (free_list == NULL) {
    block = request_space(NULL, size);
    if (!block) {
      return NULL;
    }

    free_list = block;
  } else {
    Block *last = free_list;

    block = find_free_block(&last, size);
    if (!block) {
      block = request_space(last, size);
      if (!block) {
        return NULL;
      }
    } else
    {
      block->free = false;
    }
  }

  return (block + 1);
}

void my_free(void *ptr) {
  if (!ptr) {
    printf("Empty pointer\n");
    return;
  }

  Block *block = (Block *)ptr - 1;
  Block *pre_block = (Block *)ptr - 1;
  block->free = true;


  // coalescing logic
  if (block->next && block->next->free) {
    block->size += BLOCK_SIZE + block->next->size;
    block->next = block->next->next;
    if (block->next) {
      block->next->prev = block;
    }
  }

  if (pre_block->prev && pre_block->prev->free) {
    pre_block->size += BLOCK_SIZE + pre_block->prev->size;
    pre_block->prev = pre_block->prev->prev;
    if (pre_block->prev) {
      pre_block->prev->next = pre_block;
    }
  } else {
    if (!free_list || block < free_list) {
      free_list = block;
    }
  }
}
