#include "object_pool.h"
	
void pool_bindto(struct list_objpool_header *head, void *objs) {
	head->start_obj = (struct list_objpool *)((char *)objs + head->offset);
	pool_reset(head);
}

bool pool_is_bind(struct list_objpool_header *head) {
	return ((head->capacity > 0) && (head->unit_size >= sizeof(struct list_objpool)) && 
	(head->start_obj != NULL)) ? true : false;
}

void pool_reset(struct list_objpool_header *head) {
	if(pool_is_bind(head)) {
		int i = 0;
		head->available_obj.next_available = head->start_obj;
		struct list_objpool *next = head->start_obj;
		for(; i < (head->capacity - 1); i++) {			
			next->next_available = (struct list_objpool *)((char *)next + head->unit_size);
			next = next->next_available;
		}
		next->next_available = &(head->available_obj);
		head->allocated = 0;
	}
}

void *pool_alloc(struct list_objpool_header *head) {
	struct list_objpool *next = head->available_obj.next_available;
	void *ptr = NULL;
	if(next != &(head->available_obj)) {
		ptr = (void *)((char *)next - head->offset);
		head->available_obj.next_available = next->next_available;
		next->next_available = NULL;	/* Marking allocated object */
		head->allocated++;
	}
	return ptr;
}

void pool_free(struct list_objpool_header *head, void *ptr) {
	if(ptr != NULL) {
		struct list_objpool *next = (struct list_objpool *)((char *)ptr + head->offset);
		if(next->next_available == NULL) {	/* Checking allocated object */
			next->next_available = head->available_obj.next_available;
			head->available_obj.next_available = next;
			head->allocated--;
		}
	}
}

bool pool_is_member(struct list_objpool_header *head, void *ptr) {
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

static bool ptr_is_alloc(struct list_objpool_header *head, void *ptr) {
	struct list_objpool *next = (struct list_objpool *)((char *)ptr + head->offset);
	return (next->next_available == NULL) ? true : false;
}	

bool pool_is_alloc(struct list_objpool_header *head, void *ptr) {
	if((ptr != NULL) && pool_is_member(head, ptr)) {
		return ptr_is_alloc(head, ptr);
	} else {
		return false;
	}
}

bool pool_is_free(struct list_objpool_header *head, void *ptr) {
	if((ptr != NULL) && pool_is_member(head, ptr)) {
		return !ptr_is_alloc(head, ptr);
	} else {
		return false;
	}
}

const size_t pool_capacity(struct list_objpool_header *head) {
	return head->capacity;
}

const size_t pool_size(struct list_objpool_header *head) {
	return head->allocated;
}

bool pool_is_full(struct list_objpool_header *head) {
	return (head->capacity == head->allocated) ? true : false;
}

bool pool_is_empty(struct list_objpool_header *head) {
	return (head->allocated == 0) ? true : false;
}
