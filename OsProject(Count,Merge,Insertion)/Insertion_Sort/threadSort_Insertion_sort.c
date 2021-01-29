#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 512
#define NUM_THREADS 3

int unsorted[SIZE];
int sorted[SIZE];

struct merge {
    int begin;
    int mid;
    int end;
};

struct insertionSort {
    int start;
    int end;
};

void fillArray(int size) {
    int i,n;
    srand(time(NULL));
    for (i=0; i<size; i++)	
    {	 
	//scanf("%d",&n);
        unsorted[i] = rand() % 100;
	//unsorted[i] = n;
    }
}

/**
 * Merges two sorted halves of the unsorted[] array into sorted[]
 *
 * begin: index of first element in first sublist
 * mid: index of the first element in the second sublist
 * end: index of the last element in the second sublist
 */
void* merge(void *args) 
{
    //unpack parameters
    struct merge *params = (struct merge*) args;
    int begin = params->begin,
    mid = params->mid,
    end = params->end;


    int i = begin, j = mid, tpos = begin;

    while (i < mid && j <= end)
    {
        if (unsorted[i] < unsorted[j])
            sorted[tpos++] = unsorted[i++];
        else
            sorted[tpos++] = unsorted[j++];
    }

    //still elements left over in first list. copy over
    while (i < mid)
        sorted[tpos++] = unsorted[i++];

    //still elements left over in first list. copy over
    while (j <= end)
        sorted[tpos++] = unsorted[j++];
    return NULL;
}

/**
 * Insertion sort
 * Two parameters used according to threads	
 * First Thread's parameter : index of the first element in unsorted sublist
 * First Thread's parameter : index of the last element in unsorted sublist
 */
void* InsertionSort(void *args)
{
    struct insertionSort *params = (struct insertionSort*) args;
    int start = params->start,
    end = params->end;

    int i = start, j, itemToInsert;

    //everything to the left of i is sorted
    while (i <= end)
    {
        itemToInsert = unsorted[i]; //a must, or else unsorted[i] gets overwritten when shifting elements

        //since everything in this sequence is sorted,starting from i, and going in reverse order, shift the elements to the right until an element less than unsorted[i] is found
        j = i-1;
        while (j >= start && itemToInsert < unsorted[j])
        {
            unsorted[j+1] = unsorted[j];
            j--;
        }
        //insert the element into s[j]
        unsorted[j+1] = itemToInsert;
        i++;
    }
    return NULL;
}

void printArray(int *list, int size) {
    int i;
    for (i=0; i<size-1; i++)
        printf("%d, ", list[i]);
    printf("%d\n", list[i]);
}

int main() {

    //fill the unsorted array
    fillArray(SIZE);
	printf("\nUnSorted Array : \n");
    printArray(unsorted, SIZE);

    //define the indices of the two sublists
    int start1 = 0,
    end1 = SIZE/2,
    start2 = end1+1,
    end2 = SIZE-1;

    //parameters for calculating time
    clock_t startingTime, endingTime;

    //2 sorting threads, and 1 merge thread
    pthread_t threads[NUM_THREADS];
    
    startingTime=clock();

    //prepare sorting params and fire off sorting threads
    struct insertionSort s[2];
    s[0].start = start1;
    s[0].end = end1;
    pthread_create(&threads[0], NULL, InsertionSort, &s[0]); //deal with first sublist

    s[1].start = start2;
    s[1].end = end2;
    pthread_create(&threads[1], NULL, InsertionSort, &s[1]); //deal with second sublist

    //wait for sorting threads to terminate
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    //prepare params and fire off merging thread
    struct merge m;
    m.begin = start1;
    m.mid = start2;
    m.end = end2;
    pthread_create(&threads[2], NULL, merge, &m); //merge the sublists into  sorted[]!

    //wait for merging thread to terminate
    pthread_join(threads[2], NULL);
	
    /* end timing */	
    endingTime =clock();	
    
    //main thread prints out the sorted array
    printf("\nSorted Array : \n");
    printArray(sorted, SIZE);
    printf("\nTime taken for execution: %f seconds\n", (endingTime -startingTime)/(double)CLOCKS_PER_SEC);

    return 0;
}
