#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#define RANGE 1000
#define SIZE 1000001
#define NUM_THREADS 3

int unsorted[SIZE];
int a[SIZE];

void fillArray() {
    int i,n;
    srand(time(NULL));
    for (i=0; i<SIZE; i++)
    {
        unsorted[i] = rand() % 100;
	while (unsorted[i]==0)
	{
		unsorted[i] = rand() % 100;
	}
    }

    printf("Unsorted Array \n");
    i = 0;
    for (i=0; i<SIZE; i++)
        printf("%d, ", unsorted[i]);
}

// The main function that sort the given string arr[] in
// alphabatical order
void* countSort(void* args) 
{ 
 int *arr =(int *)args;
    // The output character array that will have sorted arr 
   // int output[sizeof(arr)/sizeof(arr[0])]; 
     int output[SIZE]; 
  
    // Create a count array to store count of inidividul 
    // characters and initialize count array as 0 
    int count[RANGE + 1], i; 
    memset(count, 0, sizeof(count)); 
  
    // Store count of each character 
    for(i = 0; arr[i]; ++i) 
        ++count[arr[i]]; 
  
    // Change count[i] so that count[i] now contains actual 
    // position of this character in output array 
    for (i = 1; i <= RANGE; ++i) 
        count[i] += count[i-1]; 
  
    // Build the output character array 
    for (i = 0; arr[i]; ++i) 
    { 
        output[count[arr[i]]-1] = arr[i]; 
        --count[arr[i]]; 
    } 
    // Copy the output array to arr, so that arr now 
    // contains sorted characters 
    for (i = 0; arr[i]; ++i) 
        arr[i] = output[i]; 
	
	
    for (i = 0; arr[i]; ++i) 
        a[i] = arr[i];
	return NULL; 
} 

void printArray(int *list) {
    int i;
    for (i=0; i<SIZE-1; i++)
        printf("%d, ", a[i]);
}

int main() {

pthread_t threads;
clock_t startingTime, endingTime;
    fillArray();
    startingTime=clock();

pthread_create(&threads,0,countSort,(void*) unsorted);
pthread_join(threads,NULL);

endingTime =clock();

printf("\nsorted array\n");
printArray(a);
printf("\nTime taken for execution: %f seconds\n", (endingTime -startingTime)/(double)CLOCKS_PER_SEC);

return 0;
}