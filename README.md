# Custom Memory Allocator
#### Video Demo:  https://youtu.be/F-bse-hX564

## Introduction

This is a custom memory allocator written in C. It provides functionality similar to `malloc` and `free` from the standard C library. The allocator manages memory dynamically, allowing users to allocate and deallocate memory as needed during program execution. Understanding how memory allocation works at a low level is fundamental in computer science, and this project aims to provide insights into the inner workings of memory management.

## Overview

Before starting the project, I had no idea how `malloc` or even how computer memory worked. After spending some time on YouTube, I happened to find Low Level Learning’s video, “[i wrote my own memory allocator in C to prove a point](https://youtu.be/CulF4YQt6zA?si=vJK9Lrn9nG4yKsHq)”. This video sparked a sudden interest in me for understanding how memory worked, and I immediately decided to build my own `malloc` as my CS50x final project.

To understand the concepts of stack and heap, I referred to articles and videos explaining these fundamental concepts. Eventually, I found two videos, one from [Jonathan Muckell from University at Albany](https://youtu.be/jKcg3ze10Hk?si=uYiXbnOkAQo6oZtn), the other one from [Chris Kanich at University of Illinois Chicago](https://youtu.be/UTii4dyhR5c?si=Q_Z6jbC02DDZ21FZ). The former perfectly explained how the stack, the heap, and static memory worked, while the latter described the design of memory allocators.

Using the Linux man pages, I figured out how to use system calls for more memory. I could either ask the system to map a region of memory to make it ready to use using `mmap` or I could use `sbrk` to increase the data segment. I chose the latter because the process was closer to what I learned from the previously mentioned media.

## Implementation Details

### Libraries

I used the standard libraries `stdio.h` for debugging, `unistd.h` for system calls, `errno.h` for error handling, as well as `stdbool.h` for convenient Boolean representation.

### Block Structure

I implemented a structure called `Block` to represent the chunks of memory that the program is using or has used. Each `Block` consists of:
- `size_t size`: Size of the block in bytes.
- `bool free`: Availability of the block.
- `struct Block next`: Pointer to the following free block.
- `struct Block prev`: Pointer to the previous free block.

For convenience, I defined a constant `BLOCK_SIZE` for a block’s default size in bytes, as well as a variable `free_list`, a pointer to the first free block.

### Functions

To make the implementation more manageable, I created the following utility functions:
- `find_free_block`: Traverses the linked list of free blocks and returns the first block that is more or equal to the size argument.
- `request_space`: Core function of `my_malloc`. It retrieves the previous program break using `sbrk(0)` and then increases the program’s data space with `sbrk(size + BLOCK_SIZE)`. Finally, it initializes and returns the new block.

The main functions are:
- `my_malloc`: Allocates memory. It checks if any chunks exist. If not, it requests space using `request_space` and assigns the newly created block to `free_list`. Otherwise, it calls `find_free_block`, checks if the return value is not a block, calls `request_space` if it is the case. If not, it sets the returned block’s free to false. Finally, `my_malloc` returns the allocated block.
- `my_free`: Frees memory. It sets the block’s free to true and performs coalescing logic to avoid fragmentation of the heap into multiple low-bytes blocks.

## Usage

Here's a simple example of how to use `my_malloc` and `my_free`:

```c
int main() {
    // Allocate memory
    void *ptr = my_malloc(100);

    // Use allocated memory
    // ...

    // Free memory
    my_free(ptr);

    return 0;
}
```
## Error Handling
This memory allocator handles errors gracefully. It checks for out-of-memory situations and invalid input to ensure reliable performance.

## Conclusion
In conclusion, this custom memory allocator project has been a challenging but rewarding experience. By delving into the intricacies of memory management in C, I have gained a deeper understanding of how computer memory works at a low level. This project has not only enhanced my programming skills but also provided me with valuable insights into the inner workings of memory allocation algorithms. As I continue to develop and refine this memory allocator, I look forward to exploring more advanced concepts and optimizations to further improve its efficiency and functionality.

#### Feel free to adjust or add more content as needed!
