#include "object_pool.h"
	
void objpool_bindto(struct objpool_list_header *head, void *objs) {
	head->start_obj = (struct objpool_list *)((char *)objs + head->offset);
	objpool_reset(head);
}

bool objpool_is_bind(struct objpool_list_header *head) {
	return ((head->capacity > 0) && (head->unit_size >= sizeof(struct objpool_list)) && 
	(head->start_obj != NULL)) ? true : false;
}

void objpool_reset(struct objpool_list_header *head) {
	if(objpool_is_bind(head)) {
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

void *objpool_alloc(struct objpool_list_header *head) {
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

void objpool_free(struct objpool_list_header *head, void *ptr) {
	if(ptr != NULL) {
		struct objpool_list *next = (struct objpool_list *)((char *)ptr + head->offset);
		if(next->next_available == NULL) {	/* Checking allocated object */
			next->next_available = head->available_obj.next_available;
			head->available_obj.next_available = next;
			head->allocated--;
		}
	}
}

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

static bool objpool_ptr_is_alloc(struct objpool_list_header *head, void *ptr) {
	struct objpool_list *next = (struct objpool_list *)((char *)ptr + head->offset);
	return (next->next_available == NULL) ? true : false;
}	

bool objpool_is_alloc(struct objpool_list_header *head, void *ptr) {
	if(objpool_is_member(head, ptr)) {
		return objpool_ptr_is_alloc(head, ptr);
	} else {
		return false;
	}
}

bool objpool_is_free(struct objpool_list_header *head, void *ptr) {
	if(objpool_is_member(head, ptr)) {
		return !objpool_ptr_is_alloc(head, ptr);
	} else {
		return false;
	}
}

const size_t objpool_capacity(struct objpool_list_header *head) {
	return head->capacity;
}

const size_t objpool_size(struct objpool_list_header *head) {
	return head->allocated;
}

bool objpool_is_full(struct objpool_list_header *head) {
	return (head->capacity == head->allocated) ? true : false;
}

bool objpool_is_empty(struct objpool_list_header *head) {
	return (head->allocated == 0) ? true : false;
}
