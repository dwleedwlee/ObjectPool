/*
  The MIT License (MIT)
  
  Copyright (c) 2022 Dongwook Lee (elight.lee@gmail.com)
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "object_pool.h"

/**
 * objpool_is_init_head - checks whether a header of object pool is initialized
 * @head: object pool head to check
 */	
static bool objpool_is_init_head(struct objpool_list_header *head) {
	return ((head->capacity > 0) && (head->unit_size >= sizeof(struct objpool_list)) && 
	(head->start_obj != NULL)) ? true : false;
}

/**
 * objpool_init - initialize the object pool 
 * @head: object pool head to initialize
 *
 * Initialize the object pool by chaining the list pointer of the object pool. 
 */	
void objpool_init(struct objpool_list_header *head) {
	if(objpool_is_init_head(head)) {
		int i = 0;
		head->available_obj.next_available = head->start_obj;
		struct objpool_list *next = head->start_obj;
		for(; i < (head->capacity - 1); i++) {			
			next->next_available = (struct objpool_list *)((char *)next + head->unit_size);
			next = next->next_available;
		}
		next->next_available = &(head->available_obj);
		head->allocated = 0;
	}
}

/**
 * objpool_is_member - checks whether an object is a member of the object pool
 * @head: object pool head
 * @ptr: pointer of the object to check
 */	
bool objpool_is_member(struct objpool_list_header *head, void *ptr) {
	if(ptr != NULL) {
		char *_ptr = (char *)ptr;
		char *first = (char *)head->start_obj - head->offset;
		char *last = first + (head->unit_size * (head->capacity - 1));
		if((_ptr >= first) && (_ptr <= last) && ((_ptr - first) % head->unit_size) == 0) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

/**
 * objpool_alloc - allocate an object from the object pool 
 * @head: object pool head to allocate an object
 *
 * Allocate an object from the object pool and return the address.
 */	
void *const objpool_alloc(struct objpool_list_header *head) {
	struct objpool_list *next = head->available_obj.next_available;
	void *ptr = NULL;
	if(next != &(head->available_obj)) {
		ptr = (void *)((char *)next - head->offset);
		head->available_obj.next_available = next->next_available;
		next->next_available = NULL;	/* Marking allocated object */
		head->allocated++;
	}
	return ptr;
}

/**
 * objpool_free - free an object from the object pool 
 * @head: object pool head
 * @ptr: pointer of the object to free
 *
 * Free an object from the object pool only if the object is a member of the object pool
 */
void objpool_free(struct objpool_list_header *head, void *ptr) {
	if(objpool_is_member(head, ptr)) {
		struct objpool_list *next = (struct objpool_list *)((char *)ptr + head->offset);
		if(next->next_available == NULL) {	/* Checking allocated object */
			next->next_available = head->available_obj.next_available;
			head->available_obj.next_available = next;
			head->allocated--;
		}
	}
}

/**
 * objpool_ptr_is_alloc - checks whether an object is an allocated state
 * @head: object pool head
 * @ptr: pointer of the object to check
 */	
static bool objpool_ptr_is_alloc(struct objpool_list_header *head, void *ptr) {
	struct objpool_list *next = (struct objpool_list *)((char *)ptr + head->offset);
	return (next->next_available == NULL) ? true : false;
}	

/**
 * objpool_is_alloc - checks whether an object is a member of object pool and is an allocated state
 * @head: object pool head
 * @ptr: pointer of the object to check
 */	
bool objpool_is_alloc(struct objpool_list_header *head, void *ptr) {
	if(objpool_is_member(head, ptr)) {
		return objpool_ptr_is_alloc(head, ptr);
	} else {
		return false;
	}
}

/**
 * objpool_is_free - checks whether an object is a member of object pool and is a free state
 * @head: object pool head
 * @ptr: pointer of the object to check
 */
bool objpool_is_free(struct objpool_list_header *head, void *ptr) {
	if(objpool_is_member(head, ptr)) {
		return !objpool_ptr_is_alloc(head, ptr);
	} else {
		return false;
	}
}

/**
 * objpool_capacity - return total number of elements of the object pool
 * @head: object pool head
 */	
const size_t objpool_capacity(struct objpool_list_header *head) {
	return head->capacity;
}

/**
 * objpool_size - return the number of allocated elements of the object pool
 * @head: object pool head
 */
const size_t objpool_size(struct objpool_list_header *head) {
	return head->allocated;
}

/**
 * objpool_addr - return the start address of the object pool
 * @head: object pool head
 */
void *const objpool_addr(struct objpool_list_header *head) {
	return head->start_obj;
}

/**
 * objpool_is_full - checks whether the object pool is full allocated
 * @head: object pool head
 */
bool objpool_is_full(struct objpool_list_header *head) {
	return (head->capacity == head->allocated) ? true : false;
}

/**
 * objpool_is_empty - checks whether the object pool has no allocated object
 * @head: object pool head
 */
bool objpool_is_empty(struct objpool_list_header *head) {
	return (head->allocated == 0) ? true : false;
}
