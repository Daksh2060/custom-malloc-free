#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

// Adjust this value to increase the initial block size.
#define TOTAL_BLOCK 1000

// Doubly linked list data structure to hold block information.
struct block
{
    bool free;          // Flag indicating whether the block is free or allocated
    struct block *next; // Pointer to the next block in the linked list
    struct block *prev; // Pointer to the previous block in the linked list
    size_t size;        // Size of the memory block
};

// Declare arrays for both the list of free blocks and the initial memory block.
extern struct block *block_list; // Head of the linked list of memory blocks
extern char *allocated_block;    // Pointer to the entire allocated memory block

// Declare arrays for both list of free blocks and initial memory block.
extern struct block *block_list;
extern char *allocated_block;

// Initilizes block of memory to be divided up my_malloc.
void mem_init();

// Finds suitable blocks for allocation size, if not, splits blocks for better fit.
// Returns pointer to memory, or NULL on fail
void *my_malloc(size_t allocation_size);

// Frees block of memory and combines with adjacent free blocks.
// Has no return, just prints warning message
void my_free(void *free_item);

// Allows user to free initial block of memory.
void free_block();

#endif
