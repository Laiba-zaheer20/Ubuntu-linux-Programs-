#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#define RANGE 255

int a[RANGE];
void insertionSort(int arr[], int n) 
{ 
	int i, temp, j; 
	
	pid_t pid;
	pid = fork();
	if (pid<0)
	{
	 	// child proc not created
	 	perror("Child Process not created\n");
	 	_exit(-1);
	}
	
	else if (pid==0)
	{
		printf("Process running\n");
		for (i = 0; i < n; i++) 
		{	 
			j = i; 
			temp = arr[i]; 

		/* Move elements of arr[0..i-1], that are 
		greater than key, to one position ahead 
		of their current position */
			while (j > 0 && arr[j-1] > temp) 
			{ 
				arr[j] = arr[j-1]; 
				j--; 
			} 
			arr[j] = temp; 
		}
	}
	
	int status;
	 
	// Wait for child processes to finish
	waitpid(pid, &status, 0);
} 

void isSorted(int arr[], int len)
{
	if (len==1)
	{
	 	printf("Sorting Done Successfully\n");
	 	return;
	}
	 
	int i;
	for (i=1; i<len; i++)
	{
	 	if (arr[i]<arr[i-1])
	 	{
		 	printf("Sorting Not Done\n");
		 	return;
		}
	}
	 
	printf("Sorting Done Successfully\n");
	return;
}

void TakingInput(int arr[], int len)
{
	// Create randomly generated array
	int i;
	for (i=0; i<len; i++)
	{
		arr[i] = rand()%100;
		printf ("%d ",arr[i]);
	}
	
	/* taking manually input in array */
	//int i;
	//for (i=0; i<len; i++)
	//{
	//	scanf("%d",&arr[i]);
	//}
	
	//	printf("\n");
	//for (i=0; i<len; i++)
	//{
	//	printf("%d ",arr[i]);
	//}

	return;
} 

int main()
{
	int shmid;
	key_t key = IPC_PRIVATE;
	int *shm_array;
 
	int length;
	 
	printf("Enter No of elements of Array:");
	scanf("%d",&length); 
	
	size_t SHM_SIZE = sizeof(int)*length;
	
	if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
	{
	 	perror("shmget");
	 	_exit(1);
	}
	
	if ((shm_array = shmat(shmid, NULL, 0)) == (int *) -1)
	{
	 	perror("shmat");
	 	_exit(1);
	}
	
	srand(time(NULL));
	TakingInput(shm_array, length);
	printf ("\n");
	// startingTime and endingTime for calculating time for Sorting
	clock_t startingTime, endingTime;
	startingTime=clock();
	
	insertionSort(shm_array, length);
	endingTime = clock();
	isSorted(shm_array, length);
	
	for (int i=0;i<length;i++)
	{
		printf ("%d ",shm_array[i]);
	}
		printf ("\n");
	
	
	// calculating time taken in applying Sorting
	printf("Time taken in execution: %f\n",(endingTime - startingTime)/(double)CLOCKS_PER_SEC);
	 
	return 0;
}
