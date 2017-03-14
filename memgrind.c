#include "mymalloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

// A: 1000 separate malloc()s of 1 byte, then free() the 1000 1 byte pointers one by one
double A() {
	struct timeval start, stop;
	void* ptr_array[1000];
	int i;

	gettimeofday(&start, NULL);

	for(i = 0; i < 1000; i++){
		ptr_array[i] = malloc(1);
	}

	for(i = 0; i < 1000; i++){
		free(ptr_array[i]);
	}

	gettimeofday(&stop, NULL);

	double totalTime = (((stop.tv_sec - start.tv_sec) * 1000000.0) + (stop.tv_usec - start.tv_usec));
	return totalTime;
}

//B: first malloc() 1 byte and immediately free it - do this 1000 times
double B() {
	struct timeval start, stop;
	int i;

	gettimeofday(&start, NULL);

	for(i = 0; i < 1000; i++){
		void* ptr = malloc(1);
		free(ptr);
	}

	gettimeofday(&stop, NULL);

	double totalTime = (((stop.tv_sec - start.tv_sec) * 1000000.0) + (stop.tv_usec - start.tv_usec));
	return totalTime;
}

//C: Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer - do this 1000 times
double C() {
	void* ptr_array[1000];
	int malloc_counter = 0, size = 0, randnum;
	struct timeval start, stop;

	gettimeofday(&start, NULL);

	while(malloc_counter < 1000){
		randnum = rand() % 2;

		//if randnum is 0 or if the array is empty, malloc a new byte
		if(randnum == 0 || size == 0){
			ptr_array[size] = malloc(1);
			malloc_counter++;
			size++;
		}
		//otherwise, choose a random entry and free it from the size of the array, then fill in the gap with the top entry
		else{
			int entry = rand() % size;

			// randomly chose to free the ptr at the size
			if(entry == (size - 1)){
				free(ptr_array[entry]);
				ptr_array[entry] = 0;
			}
			else{
				free(ptr_array[entry]);
				ptr_array[entry] = ptr_array[size-1];
				ptr_array[size-1] = 0;
			}
			size--;
		}
	}

	//free any remaining pointers
	while(size > 0){
		free(ptr_array[size-1]);
		size--;
	}

	gettimeofday(&stop, NULL);

	double totalTime = (((stop.tv_sec - start.tv_sec) * 1000000.0) + (stop.tv_usec - start.tv_usec));
	return totalTime;
}

double D() {
	void* ptr_array[1000];
	int randSizes[1000];
	int malloc_counter = 0, totalRequests = 0,  size = 0, randnum, structSize = 32, randSize;
	struct timeval start, stop;

	gettimeofday(&start, NULL);

	while(malloc_counter < 1000){
		randnum = rand() % 2;
		//if randnum is 0 or if the array is empty, malloc a new byte
		if(randnum == 0 || size == 0){
			// select random size mallocs from 0 bytes to 64 bytes
			randSize = (rand() % 64) + 1;
			// make sure that its not over 5000
			if((totalRequests + structSize + randSize) < 5000){
				totalRequests += (structSize + randSize);
				randSizes[size] = randSize; // save the randomSize in the same index as ptr saved in array
				ptr_array[size] = malloc(randSize);
				malloc_counter++;
				size++;
			}
		}
		//otherwise, choose a random entry and free it from the size of the array, then fill in the gap with the top entry
		else {
			int entry = rand() % size;
			randSize = randSizes[entry]; // get back the size of the memory that ptr is pointing to

			// randomly chose to free the ptr at the size
			if(entry == (size - 1)){
				free(ptr_array[entry]);

				ptr_array[entry] = 0;
				randSizes[entry] = 0;
			}
			// chose to free a pointer from the middle
			else{
				free(ptr_array[entry]);
				ptr_array[entry] = ptr_array[size-1];
				ptr_array[size-1] = 0;

				randSizes[entry] = randSizes[size-1];
				randSizes[size-1] = 0;
			}
			size--;
			totalRequests -= (structSize + randSize);
		}
	}

	//free any remaining pointers
	while(size > 0){
		free(ptr_array[size-1]);
		randSizes[size-1]=0;
		size--;
	}

	gettimeofday(&stop, NULL);

	double totalTime = (((stop.tv_sec - start.tv_sec) * 1000000.0) + (stop.tv_usec - start.tv_usec));
	return totalTime;
}

//E: ptr1 < ptr3 < ptr2 and malloc in order ptr1, ptr2, ptr3
// and then free ptr2 first because its in the middle then ptr1, and ptr3 in order
double E() {
    void* ptr_array[1000];
    struct timeval start, stop;

    gettimeofday(&start, NULL);
    int i;

    for(i = 0; i < 1000; i++){
		char* ptr1; 
        char* ptr2;
		char* ptr3;
		ptr1 = malloc(256);
		ptr2 = malloc(256);
		ptr3 = malloc(256);
		free(ptr1);
		free(ptr2);
		free(ptr3);
    }

    gettimeofday(&stop, NULL);

    double totalTime = ((stop.tv_sec-start.tv_sec)*1000000.0 + stop.tv_usec-start.tv_usec);
    return totalTime;
}

//F: malloc() randomized byte sizes ranging from 1 to 1024 and continue to do so until
//the memory is within 1024 bytes of 5000 bytes. Once this point is reached, it frees all pointers.
double F() {
    void* ptr_array[1000];
    int randSize=0, size=0, structSize = 32, totalRequests=0, i=0;
    struct timeval start, stop;

    gettimeofday(&start, NULL);

	for(i = 0; i<1000; i++){
		// fills memory with randomly generated byte sizes
		do{
			randSize = (rand() % 1024) +1;
			totalRequests += (structSize + randSize);
			ptr_array[size] = malloc(randSize);
			size++;
		}while(totalRequests<=3976);       
		// exits when the next potential byte size added causes memory capcity to 
		// be within 1024 bytes of 50000

		//free any remaining pointers
		while(size > 0){
			free(ptr_array[size-1]);
			size--;
		}
	}

    gettimeofday(&stop, NULL);

    double totalTime = (((stop.tv_sec - start.tv_sec) * 1000000.0) + (stop.tv_usec - start.tv_usec));
    return totalTime;
}

int main(int argc, char* argv[]) {
	double workloadA = 0,workloadB = 0, workloadC = 0,workloadD = 0,workloadE = 0,workloadF = 0;
	int i;
	
	for(i = 0; i<100 ; i++){
		workloadA += A();
		workloadB += B();
		workloadC += C();
		workloadD += D();
		workloadE += E();
		workloadF += F();
	}

	printf("workloadA: %f\n", workloadA/100);
	printf("workloadB: %f\n", workloadB/100);
	printf("workloadC: %f\n", workloadC/100);
	printf("workloadD: %f\n", workloadD/100);
	printf("workloadE: %f\n", workloadE/100);
	printf("workloadF: %f\n", workloadF/100);
}
