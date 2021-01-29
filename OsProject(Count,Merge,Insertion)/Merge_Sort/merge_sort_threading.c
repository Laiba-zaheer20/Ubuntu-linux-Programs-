#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

/* define variables for the problem */
#define SEED  100
/* #define LENGTH 100000 (use when we want to work with random function) */
#define LENGTH 32
#define UPPER_LIM 10000
#define LOWER_LIM  1
#define NUM_THREADS 2


/* define derived values from the variables */
const int no_of_threads = LENGTH / NUM_THREADS;
const int offset = LENGTH % NUM_THREADS;
int arr[LENGTH];


/* generate random numbers within the specified limit*/
int generate_random_number(unsigned int lower_limit, unsigned int upper_limit) {
    return lower_limit + (upper_limit - lower_limit) * ((double)rand() / RAND_MAX);
}

/* merge function */
void merge(int arr[], int left, int middle, int right) {
    int i = 0;
    int j = 0;
    int k = 0;
    int left_length = middle - left + 1;
    int right_length = right - middle;
    int left_array[left_length];
    int right_array[right_length];
    
    /* copy values to left array */
    for (int i = 0; i < left_length; i ++) {
        left_array[i] = arr[left + i];
    }
    
    /* copy values to right array */
    for (int j = 0; j < right_length; j ++) {
        right_array[j] = arr[middle + 1 + j];
    }
    
    i = 0;
    j = 0;
    /** chose from right and left arrays and copy */
    while (i < left_length && j < right_length) {
        if (left_array[i] <= right_array[j]) {
            arr[left + k] = left_array[i];
            i ++;
        } else {
            arr[left + k] = right_array[j];
            j ++;
        }
        k ++;
    }
    
    /* copy the remaining values to the array */
    while (i < left_length) {
        arr[left + k] = left_array[i];
        k ++;
        i ++;
    }
    while (j < right_length) {
        arr[left + k] = right_array[j];
        k ++;
        j ++;
    }
}
/* perform merge sort */
void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort(arr, left, middle);
        merge_sort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}

/** assigns work to each thread to perform merge sort */
void *thread_merge_sort(void* arg)
{
    int thread_id = (long)arg;
    int left = thread_id * (no_of_threads);
    int right = (thread_id + 1) * (no_of_threads) - 1;
    if (thread_id == NUM_THREADS - 1) {
        right += offset;
    }
    int middle = left + (right - left) / 2;
    if (left < right) {
        merge_sort(arr, left, right);
        merge_sort(arr, left + 1, right);
        merge(arr, left, middle, right);
    }
    return NULL;
}
/* merge locally sorted sections */
void merge_sections_of_array(int arr[], int number, int aggregate) {
    for(int i = 0; i < number; i = i + 2) {
        int left = i * (no_of_threads * aggregate);
        int right = ((i + 2) * no_of_threads * aggregate) - 1;
        int middle = left + (no_of_threads * aggregate) - 1;
        if (right >= LENGTH) {
            right = LENGTH - 1;
        }
        merge(arr, left, middle, right);
    }
    if (number / 2 >= 1) {
        merge_sections_of_array(arr, number / 2, aggregate * 2);
    }
}

/* test to ensure that the array is in sorted order */
void test_array_is_in_order(int arr[]) {
    int max = 0;
    for (int i = 1; i < LENGTH; i ++) {
        if (arr[i] >= arr[i - 1]) {
            max = arr[i];
        } else {
            printf("Error. Out of order sequence: %d found\n", arr[i]);
            return;
        }
    }
    printf("Array is in sorted order\n");
}

int main(int argc, const char * argv[]) {
    srand(SEED);
	int n;
    printf("\nEnter Number of Elements in array : \n");
    scanf("%d",n);
	LENGTH=n;
	/* initialize array with random numbers */
    for (int i = 0; i < LENGTH; i ++) {
        arr[i] = generate_random_number(LOWER_LIM, UPPER_LIM);
	//scanf("%d",&arr[i]);
    }

    printf("\nEntered Array : \n");	
    /* printing array with random numbers */
    for (int i = 0; i < LENGTH; i ++) {
        printf("%d ", arr[i]);
    }
  
    clock_t startingTime, endingTime;	
    /* begin timing */
    pthread_t threads[NUM_THREADS];
    startingTime=clock();
    
    /* create threads */
    for (long i = 0; i < NUM_THREADS; i ++) {
        int rc = pthread_create(&threads[i], NULL, thread_merge_sort, (void *)i);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
    for(long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    merge_sections_of_array(arr, NUM_THREADS, 1);
    
    /* end timing */	
    endingTime =clock();
   
    printf("\nSorted Array : \n");	
    /* printing array with random numbers */ 
    for (int i = 0; i < LENGTH; i ++)
    {
        printf("%d ", arr[i]);
    
    }	
    printf("\nTime taken for execution: %f seconds\n", (endingTime -startingTime)/(double)CLOCKS_PER_SEC);
    /* test to ensure that the array is in sorted order */
    /*test_array_is_in_order(arr); */
    return 0;
}

