#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "object_pool.h"

#define OBJECT_POOL_CAPACITY		(200)

struct my_list {
	struct list_objpool objpool;
	int i;
};

struct my_list g_node[OBJECT_POOL_CAPACITY];

LIST_OBJPOOL_HEAD(g_ObjHead, struct my_list, objpool, OBJECT_POOL_CAPACITY);

void main(void) {
	
	printf("Address of source: 0x%p\n", &g_node);
	printf("Is pool bind? : %d\n", pool_is_bind(&g_ObjHead));
	pool_bindto(&g_ObjHead, &g_node);	
	printf("Is pool bind? : %d\n", pool_is_bind(&g_ObjHead));
	
	struct my_list *list[OBJECT_POOL_CAPACITY];
	list[0] = pool_alloc(&g_ObjHead);
	list[0]->i = 24;
	
	printf("Is object member? : %d\n", pool_is_member(&g_ObjHead, list[0]));
	printf("Is object member? : %d\n", pool_is_member(&g_ObjHead, list[1]));
	
	printf("Is object alloc? : %d\n", pool_is_alloc(&g_ObjHead, list[0]));
	pool_free(&g_ObjHead, list[0]);
	printf("Is object alloc? : %d\n", pool_is_alloc(&g_ObjHead, list[0]));
	
	printf("Is object free? : %d\n", pool_is_free(&g_ObjHead, list[0]));
	printf("Is object free? : %d\n", pool_is_free(&g_ObjHead, list[1]));
	
	printf("Is pool empty? : %d\n", pool_is_empty(&g_ObjHead));
	for(int i = 0; i < OBJECT_POOL_CAPACITY; i++) {
		list[i] = pool_alloc(&g_ObjHead);
		list[i]->i = i;
	}
	printf("Is pool full? : %d\n", pool_is_full(&g_ObjHead));
	pool_reset(&g_ObjHead);
	printf("Is pool empty? : %d\n", pool_is_empty(&g_ObjHead));
	
	srand((unsigned int)time(NULL));
	for(int i = 0; i < OBJECT_POOL_CAPACITY; i++) {
		int n = rand() % 2;
		if(n == 0) { list[i] = pool_alloc(&g_ObjHead); }
	}
	
	for(int i = 0; i < OBJECT_POOL_CAPACITY; i++) {
		int n = rand() % 2;
		if(n == 0) { pool_free(&g_ObjHead, list[i]); }
	}
	
	printf("Capacity of pool : %d\n", pool_capacity(&g_ObjHead));
	printf("Size of pool : %d\n", pool_size(&g_ObjHead));
}
