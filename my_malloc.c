#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "my_malloc.h"

/**
 * @brief Structure representing a memory block.
 */
struct block
{
    size_t size;        /**< Size of the memory block */
    bool free;          /**< Flag indicating whether the block is free or allocated */
    struct block *next; /**< Pointer to the next block in the linked list */
    struct block *prev; /**< Pointer to the previous block in the linked list */
};

struct block *block_list;              /**< Head of the linked list of memory blocks */
char *allocated_block;                 /**< Pointer to the entire allocated memory block */
int block_size = sizeof(struct block); /**< Size of the block structure */

/**
 * @brief Initializes the custom memory allocation system.
 *
 * This function allocates the main memory block and sets up the initial block in the linked list.
 */
void mem_init()
{
    // Allocate memory for the entire block
    allocated_block = malloc(TOTAL_BLOCK);

    if (allocated_block == NULL)
    {
        // Handle failure to allocate memory
        printf("Could not allocate main memory block\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the block_list to point to the allocated memory
    block_list = (struct block *)allocated_block;

    // Initialize the properties of the initial block
    block_list->size = TOTAL_BLOCK - block_size;
    block_list->free = true;
    block_list->next = NULL;
    block_list->prev = NULL;
}

/**
 * @brief Allocates a block of memory.
 *
 * This function searches for a suitable free block in the linked list or splits a larger block to fulfill the request.
 *
 * @param allocation_size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block, or NULL if there is not enough space.
 */
void *my_malloc(size_t allocation_size)
{
    // Check if mem_init() has been called
    if (block_list == NULL)
    {
        printf("Please run mem_init() before starting\n");
        return NULL;
    }

    // Traverse the linked list to find a suitable free block
    struct block *current = block_list;

    while (((current->free == false) && (current->next != NULL)) || current->size < allocation_size)
    {
        current = current->next;
    }

    // Calculate the total space required, including block metadata
    size_t space_required = allocation_size + block_size;

    // Case 1: Exact match, allocate the entire block
    if (allocation_size == current->size)
    {
        current->free = false;
        current += 1;
        return (void *)(current);
    }
    // Case 2: Split the block and allocate the requested size
    else if (space_required < current->size)
    {
        struct block *new_block = (void *)((void *)current + space_required);

        // Set properties for the new block
        new_block->size = ((current->size) - allocation_size - block_size);
        new_block->free = true;
        new_block->next = current->next;
        new_block->prev = current;

        // Update properties for the current block
        current->size = allocation_size;
        current->free = false;
        current->next = new_block;

        current += 1;
        return (void *)(current);
    }
    // Case 3: Not enough space left
    else
    {
        printf("Not enough space left\n");
        return NULL;
    }
}

/**
 * @brief Frees a previously allocated block of memory.
 *
 * This function marks the specified block as free and may merge adjacent free blocks.
 *
 * @param free_item Pointer to the memory block to be freed.
 */
void my_free(void *free_item)
{
    void *block_start = (void *)allocated_block;
    void *block_end = block_start + TOTAL_BLOCK;

    // Check if the pointer is within the allocated memory block
    if (block_start <= free_item && free_item <= block_end)
    {
        struct block *free_block = (struct block *)free_item - 1;
        free_block->free = true;

        // Merge with the next block if it is free
        if (free_block->next && free_block->next->free)
        {
            free_block->size += (free_block->next->size + block_size);
            free_block->next = free_block->next->next;

            if (free_block->next)
            {
                free_block->next->prev = free_block;
            }
        }

        // Merge with the previous block if it is free
        if (free_block->prev && free_block->prev->free)
        {
            free_block->prev->size += (free_block->size + block_size);
            free_block->prev->next = free_block->next;

            if (free_block->next)
            {
                free_block->next->prev = free_block->prev;
            }
        }
    }
    else
    {
        // Handle invalid pointer
        printf("Please pass a pointer allocated with my_malloc\n");
    }
}

/**
 * @brief Frees the entire allocated memory block.
 *
 * This function releases the memory allocated for the entire block.
 */
void free_block()
{
    free(allocated_block);
}
