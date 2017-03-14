#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

#include <stdio.h>
#include <stdlib.h>

#define MemorySize 5000
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

// defining struct for a Doubly Linked List
struct MemBlock
{
	size_t size;
	struct MemBlock *next, *prev;
	unsigned int isfree; // 1 is true = free, 0 is false = being used

};
typedef struct MemBlock MemBlock;

void* mymalloc(unsigned int size, char*, unsigned int line);
void myfree(void* p, char* file, unsigned int line);

#endif
