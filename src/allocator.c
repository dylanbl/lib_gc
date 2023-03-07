// Dylan Lewis
// November 4, 2022
// This program implements the functionality of malloc() and free()

#include "../include/allocator.h"

#include <stdio.h>
#include <unistd.h>

Node list_head = NULL; 

void* Create(size_t size) {
	Node n, extra; 
	size_t padding, total_bytes; 

	padding = (size % 8 == 0) ? 0 : 8 - (size % 8); 

	total_bytes = size + padding + 8; 

	/* Search for an existing block with a size big enough */ 
	n = list_head; 
	while (n != NULL) {
		if (n->size >= total_bytes) {
			break; 
		}

		n = n->flink; 
	}

	/* Create new block */ 
	if (n == NULL) {
		n = _new_block(total_bytes);
	}
	/* Found existing block to use, but it needs to be split */ 
	else if (n->size - total_bytes > 8) {
		extra = n; 
		extra->size = n->size - total_bytes;
	
		n = (Node) (((void *) extra) + extra->size);
		n->size = total_bytes;
		n->flink = NULL; 
	}
	/* Found an existing block that does not need to be split */ 
	else {
		_remove_from_list(n); 
	}

	n = (Node) (((void *) n) + 8);
	return n; 
}

void Destroy(void* ptr) {
	Node curr, next;
	void *ptr_node; 

	ptr_node = ptr - 8; 

	/* Add head node to empty free list  */ 
	if (list_head == NULL) {
		list_head = (Node) ptr_node;
		return; 
	}

	curr = list_head; 
	next = curr->flink; 

	/* Find where node belongs in free list */ 
	while (next != NULL) {
		if ((void *) next > ptr_node) {
			break; 
		}

		curr = curr->flink; 
		next = next->flink; 
	}

	/* Node goes in between 2 nodes */
	if (next != NULL) {
		curr->flink = (Node) ptr_node; 
		((Node) ptr_node)->flink = next; 
	}
	/* Node goes at the end of free list */ 
	else {
		curr->flink = ((Node) ptr_node); 
	}
		
	return; 
}

void* FreeListBegin() {
	return list_head; 
}

void* FreeListNext(void* node) {
	return ((Node) node)->flink; 
}

void _coalesce_free_list() {
	Node n;

	n = list_head; 
	while (n != NULL) {
		while (((void *) n) + n->size == (void *) n->flink) {
			n->size += n->flink->size; 
			n->flink = n->flink->flink; 
		}

		n = n->flink;
	}
}

void
_remove_from_list(Node n) {
	Node curr, next;

	if (list_head == n) {
		list_head = n->flink; 
		return; 
	}

	curr = list_head; 
	next = curr->flink; 

	while (next != n) {
		curr = curr->flink; 
		next = next->flink; 
	}

	curr->flink = next->flink; 
	n->flink = NULL; 

	return;
}

Node
_allocate(size_t size) {
	Node n;

	n = (Node) sbrk(size); 
	n->size = size;
	n->flink = NULL;
	return n; 
}

Node 
_new_block(size_t size) {	
	Node n, ret; 
	
	if (size > 8192) {
		n = (Node) _allocate(size); 
		return n; 
	}

	n = (Node) _allocate(8192); 
	n->size = 8192 - size; 
	n->flink = NULL; 
	
	ret = (Node) (((void *) n) + n->size);
	ret->size = size; 
	ret->flink = NULL; 
	
	Destroy(((void *) n) + 8); 

	return ret;
}
