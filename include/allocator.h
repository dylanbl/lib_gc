// Dylan Lewis
// November 4, 2022
// This program implements the functionality of malloc() and free()

#include <stdio.h>
#include <unistd.h>

/* A free list node */ 
typedef struct node {
	unsigned int size; 
	struct node *flink; 
} * Node; 

/* 
 * @name Create
 * @brief Allocates memory on the heap for the user
 * @param[in] The number of bytes the user wants 
 * @returns A pointer to the user's allocated memory 
 */
void* Create(size_t size);

/*
 * @name Destroy
 * @brief Adds a node to the free list; insertion sort is 
 *		  performed to get the list ordered by memory address
 * @param[in] The users pointer to the block they want to free
 */
void Destroy(void* ptr); 

/*
 * @name free_list_begin
 * @brief Returns the head of the free list
 * @returns The head of the free list
 */
void* FreeListBegin();

/* 
 * @name free_list_next
 * @brief Returns the next node on the free list
 * @param[in] The node the user wants the next node from 
 * @returns The next node on the free list 
 */
void* FreeListNext(void* node);

/*
 * @name coalesce_free_list
 * @brief Combines adjacent memory on the free list
 */
void _coalesce_free_list();

/*
 * @name RemoveFromList
 * @brief Removes the given node from the free list
 * @param[in] The node to remove 
 */
void
_remove_from_list(Node n);

/*
 * @name Allocate
 * @brief Calls sbrk() and sets the size and flink fields of the new node 
 * @param[in] The number of bytes passed to sbrk()
 * @returns A pointer to the head of the new node 
 */
Node
_allocate(size_t size);

/*
 * @name NewBlock
 * @brief Creates a new block, determines what value to call 
 *		  sbrk() with and if the newly created block needs to be
 *		  split or not 
 * @param[in] The number of bytes needed 
 * @returns A pointer to the head of the new block 
 */
Node 
_new_block(size_t size); 
