#include "mymalloc.h"

//theMemory is an array of 5000 bytes
static char theMemory[MemorySize];
//root ptr of the Doubly Linked list structure
static struct MemBlock* root;

void *mymalloc(unsigned int size, char* file, unsigned int line)
{
    //printf("MALLOC\n");
    static int initialized = 0; //boolean if initialized or not
    MemBlock *currentNode;
    MemBlock *newNode;
    //if not initialized yet
    if(!initialized)
    {
    // set the root to the address of the 5000 byte memory
        root = (MemBlock *) theMemory;
    // So the root will point to the the starting of 5000 bytes and so that when someone requests 5000
        // it will allow it to provide 5000 bytes without a header
        root->size = MemorySize;
    //    printf("root->size: %i\n", (int)root->size);
    //    printf("MemBlock size: %i\n", (int)sizeof(MemBlock));
    // set the next and prev of the root to 0 indicating no prev and no next
        root->next = 0;
        root->prev = 0;
    // set isfree to 1 indicating that it is free
        //printf("root->isfree inside initializing: %u\n", root->isfree);
        root->isfree = 1;
    // now the root has been initialized, dont want to change anymore so set to 1
        initialized = 1;
        //printf("Finished getting Initialized\n");
    }
    // set another ptr named currentNode equal to the root because root ptr does not move
    currentNode = root;
    do
    {
    //printf("currentNode->isfree right inside mymalloc do loop: %u\n", currentNode->isfree);
        // should not be able to allocate 0 bytes
    if (size == 0 || currentNode == 0) {
        fprintf(stderr, "Unable to allocate bytes in FILE: %s on LINE: %d\n", file, line);
        return 0;
    }
        // size requested > currentNode block size so go to next block or block is not free
        else if(currentNode->size < size || !(currentNode->isfree))
        {
            //printf("GO TO NEXT NODE: %u\n", currentNode->isfree);
        currentNode = currentNode->next;
        }
        // Enough room for size requested but not for both the size and the header struct
        else if(currentNode->size < (size+sizeof(MemBlock)))
        {
            //printf("Allocated memory of size %d in elseif\n",size);
      //printf("currentNode->isfree in else if(currentNode->size < (size+sizeof(MemBlock))): %u\n", currentNode->isfree);
            //currentNode is no longer free
            currentNode->isfree = 0;
            currentNode = currentNode->next;
            return (char*)currentNode;
        }
        // Here theres enough for both the header and the size
        else if (currentNode->size >(size+sizeof(MemBlock)))
        {
            //printf("currentNode->size: %d\n",(int)currentNode->size);
            // make a new node called newNode that will hold the rest of
            // the memory size available starting from the currentNode
            int MemoryLocationcN = (void*)currentNode - (void *)theMemory;
            printf("MemoryLocation of currentNode: %d\n", MemoryLocationcN);
            newNode = (MemBlock*) ((char*)currentNode + sizeof(MemBlock) + size);
            int MemoryLocationnN = (void*)newNode - (void *)theMemory;
            printf("MemoryLocation of newNode: %d\n", MemoryLocationnN);
            if (MemoryLocationnN + sizeof(MemBlock) > MemorySize)
            {
                //printf("INSIDE CATCH\n");
                currentNode->isfree = 0;
                currentNode->next = 0;
                //printf("printing lastPtr at %d\n",MemoryLocationnN);
                return (char*)newNode;
            }
            else {
                //printf("INSIDE ELSE\n");
                // currentNode <- newNode -> 0
                newNode->prev = currentNode;
                newNode->next = currentNode->next;
                currentNode->next = newNode;
                newNode->size = currentNode->size - sizeof(MemBlock) - size;
          //printf("newNode->isfree in mymalloc else: %u\n", newNode->isfree);
                //printf("newNode->size: %d\n",(int)newNode->size);
                // now the newNode is free
                newNode->isfree = 1;
                // the currentNode is now being used
                currentNode->size = size;
          //printf("currentNode->isfree in mymalloc else: %u\n", currentNode->isfree);
                currentNode->isfree = 0;
                //("Allocated memory of size %d in last else\n",size);
                //printf("(void*)currentNode - (void *)theMemory < MemorySize: %d\n", (int)((void*)currentNode + sizeof(MemBlock) - (void *)theMemory)
                return (char*)currentNode + sizeof(MemBlock);
            }
        }
    } while(currentNode != 0);
    fprintf(stderr,"Error: No more space left for this in FILE: %s on LINE: %d\n", file, line);
    return 0;
}

void myfree(void *p, char *file, unsigned int line){
	// location of the node being freed
	int location = p - (void *)theMemory;
	MemBlock *currentNode;
	MemBlock *ptr = (MemBlock *)((char *)p - sizeof(MemBlock));

	currentNode = root;

	if(currentNode != NULL){
		while(currentNode != NULL){
		//		printf("freeing %u\n", currentNode->isfree);
			if(currentNode == ptr){
				// free()ing twice currentNode already == 1 which means its free
				if(currentNode->isfree != 0){
					fprintf(stderr, "Error: Cannot free same pointer twice in FILE: %s on LINE: %d\n", file, line);
					return;
				}
				//printf("currentNode->isfree in myfree while: %u\n", currentNode->isfree);
				currentNode->isfree = 1;
				// need to merge blocks so we dont have chunks of small blocks
				// Check to the Right
				if(currentNode != NULL && currentNode->next != NULL && currentNode->next->isfree){
					//printf("currentNode->next->isfree in myfree): %u\n", currentNode->next->isfree);
					currentNode->size += currentNode->next->size;
					currentNode->next = currentNode->next->next;
				}
				// Check to the Left
				if(currentNode != NULL && currentNode->prev != NULL && currentNode->prev->isfree){
				//	printf("currentNode->prev->isfree in myfree): %u\n", currentNode->prev->isfree);
					currentNode->prev->size += currentNode->size;
					currentNode->prev->next = currentNode->next;
				}
				//printf("isfree after checking both left and right: %u\n", currentNode->isfree);
				//printf("Freeing block at location %d\n", location);
				return;
			}
			currentNode = currentNode->next;
		}
	}else if(currentNode == NULL || currentNode==0){
			fprintf(stderr, "Error: Invalid pointer address cannot free in FILE: %s on LINE: %d\n", file, line);
	}
}
