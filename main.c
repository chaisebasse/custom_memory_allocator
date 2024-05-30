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

int main() {
  int* arr = (int*) my_malloc(10 * sizeof(int));
  if (arr == NULL) {
    fprintf(stderr, "my_malloc failed\n");
    return 1;
  }

  for (int i = 0; i < 10; i++) {
    arr[i] = i;
    printf("%d ", arr[i]);
  }
  printf("\n");

  my_free(arr);
  return 0;
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
  block->free = true;
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

  if (last) {
    last->next = block;
  }

  block->size = size;
  block->next = NULL;
  block->free = false;
  block->prev = last;

  return block;
}
