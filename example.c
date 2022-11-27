#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "object_pool.h"

#define OBJECT_POOL_CAPACITY		(2000)

struct my_list {
	struct objpool_list objpool;
	int i;
};

struct my_list g_node[OBJECT_POOL_CAPACITY];

OBJPOOL_LIST_HEAD(g_ObjHead, struct my_list, objpool, OBJECT_POOL_CAPACITY);

void main(void) {
	
	printf("Address of source: 0x%p\n", &g_node);
	printf("Is pool bind? : %d\n", objpool_is_bind(&g_ObjHead));
	objpool_bindto(&g_ObjHead, &g_node);	
	printf("Is pool bind? : %d\n", objpool_is_bind(&g_ObjHead));
	
	struct my_list *list[OBJECT_POOL_CAPACITY];
	list[0] = objpool_alloc(&g_ObjHead);
	list[0]->i = 24;
	
	printf("Is object member? : %d\n", objpool_is_member(&g_ObjHead, list[0]));
	printf("Is object member? : %d\n", objpool_is_member(&g_ObjHead, list[1]));
	
	printf("Is object alloc? : %d\n", objpool_is_alloc(&g_ObjHead, list[0]));
	objpool_free(&g_ObjHead, list[0]);
	printf("Is object alloc? : %d\n", objpool_is_alloc(&g_ObjHead, list[0]));
	
	printf("Is object free? : %d\n", objpool_is_free(&g_ObjHead, list[0]));
	printf("Is object free? : %d\n", objpool_is_free(&g_ObjHead, list[1]));
	
	printf("Is pool empty? : %d\n", objpool_is_empty(&g_ObjHead));
	for(int i = 0; i < OBJECT_POOL_CAPACITY; i++) {
		list[i] = objpool_alloc(&g_ObjHead);
		list[i]->i = i;
	}
	printf("Is pool full? : %d\n", objpool_is_full(&g_ObjHead));
	objpool_reset(&g_ObjHead);
	printf("Is pool empty? : %d\n", objpool_is_empty(&g_ObjHead));
	
	srand((unsigned int)time(NULL));
	for(int i = 0; i < OBJECT_POOL_CAPACITY; i++) {
		int n = rand() % 2;
		if(n == 0) { list[i] = objpool_alloc(&g_ObjHead); }
	}
	
	for(int i = 0; i < OBJECT_POOL_CAPACITY; i++) {
		int n = rand() % 2;
		if(n == 0) { objpool_free(&g_ObjHead, list[i]); }
	}
	
	printf("Capacity of pool : %d\n", objpool_capacity(&g_ObjHead));
	printf("Size of pool : %d\n", objpool_size(&g_ObjHead));
}
