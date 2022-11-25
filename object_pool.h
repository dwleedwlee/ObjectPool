#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include <stddef.h>
#include <stdbool.h>

struct list_objpool {
	struct list_objpool *next_available;
};

struct list_objpool_header {
	struct list_objpool available_obj;
	struct list_objpool *start_obj;
	size_t allocated;
	const size_t unit_size;
	const size_t offset;
	const size_t capacity;	
};

#define LIST_OBJPOOL_HEAD_INIT(name, type, member, capacity) \
	{ {&name.available_obj}, NULL, 0, sizeof(type), offsetof(type, member), capacity }

#define LIST_OBJPOOL_HEAD(name, type, member, capacity)	\
	struct list_objpool_header name = LIST_OBJPOOL_HEAD_INIT(name, type, member, capacity)

void pool_bindto(struct list_objpool_header *head, void *objs);
bool pool_is_bind(struct list_objpool_header *head);
void pool_reset(struct list_objpool_header *head);
void *pool_alloc(struct list_objpool_header *head);
void pool_free(struct list_objpool_header *head, void *ptr);
bool pool_is_member(struct list_objpool_header *head, void *ptr);
bool pool_is_alloc(struct list_objpool_header *head, void *ptr);
bool pool_is_free(struct list_objpool_header *head, void *ptr);
const size_t pool_capacity(struct list_objpool_header *head);
const size_t pool_size(struct list_objpool_header *head);
bool pool_is_full(struct list_objpool_header *head);
bool pool_is_empty(struct list_objpool_header *head);

#endif
