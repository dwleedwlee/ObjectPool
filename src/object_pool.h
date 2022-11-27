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
#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include <stddef.h>
#include <stdbool.h>

/*
 * Circular single linked list implementation.
 * The idea of constructing an object pool with generic type comes from 
 * the list.h source code of the Linux
 */

struct objpool_list {
	struct objpool_list *next_available;
};

struct objpool_list_header {
	struct objpool_list available_obj;
	struct objpool_list *start_obj;
	size_t allocated;
	const size_t unit_size;
	const size_t offset;
	const size_t capacity;	
};

#define OBJPOOL_LIST_HEAD_INIT(name, type, member, capacity) \
	{ {&name.available_obj}, NULL, 0, sizeof(type), offsetof(type, member), capacity }

/**
 * OBJPOOL_LIST_HEAD - Initialize a objpool_list_head structure
 * @name: name of the objpool_list_head to be initialized.
 * @type: type of elements which of the object pool contains
 * @member: name of 'struct objpool_list' type member of the element
 * @capacity: number of the elements (element shoule be contained in array) 
 *
 * Initializes the head of object pool to point to itself.
 */
#define OBJPOOL_LIST_HEAD(name, type, member, capacity)	\
	struct objpool_list_header name = OBJPOOL_LIST_HEAD_INIT(name, type, member, capacity)

extern void objpool_bindto(struct objpool_list_header *head, void *objs);
extern bool objpool_is_bound(struct objpool_list_header *head);
extern void objpool_reset(struct objpool_list_header *head);
extern bool objpool_is_member(struct objpool_list_header *head, void *ptr);
extern void *objpool_alloc(struct objpool_list_header *head);
extern void objpool_free(struct objpool_list_header *head, void *ptr);
extern bool objpool_is_alloc(struct objpool_list_header *head, void *ptr);
extern bool objpool_is_free(struct objpool_list_header *head, void *ptr);
extern const size_t objpool_capacity(struct objpool_list_header *head);
extern const size_t objpool_size(struct objpool_list_header *head);
extern bool objpool_is_full(struct objpool_list_header *head);
extern bool objpool_is_empty(struct objpool_list_header *head);

#endif
