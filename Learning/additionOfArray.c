#include <pthread.h>
#include <stdio.h>

#define SIZE_OF_ARRAY 6
#define NO_OF_THREADS 2

int arr[SIZE_OF_ARRAY] = {1, 2, 3, 4, 5, 6};
int partialSumOfEachThread[NO_OF_THREADS] = {0};

void* sumSmallArray(void* arg){
    int index = *(int*)arg;
    
    // SIZE_OF_ARRAY/NO_OF_THREADS will give us how many numbers are summed by each thread.
    // lets calculate start index and end index of array to be summed in this function.
    int start = index * (SIZE_OF_ARRAY/NO_OF_THREADS);
    int end = start + (SIZE_OF_ARRAY/NO_OF_THREADS);
    
    partialSumOfEachThread[index] = 0;
    for(int i=start;i<end;i++){
        partialSumOfEachThread[index] += arr[i];
    }

    // this wil terminate the thread.
    pthread_exit(NULL);
}

int main(){
    // this will create array of threads indetifier ( which will be used to pass in thread_create()).
    pthread_t threads[NO_OF_THREADS];
    int thread_ids[NO_OF_THREADS];

    for(int i=0;i<NO_OF_THREADS;i++){
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sumSmallArray, (void*)&thread_ids[i]);
    }

    for(int i=0;i<NO_OF_THREADS;i++){
        pthread_join(threads[i], NULL);
    }

    int total_sum = 0;
    for(int i=0;i<NO_OF_THREADS;i++){
        total_sum += partialSumOfEachThread[i];
    }

    printf("Total sum of array is : %d\n", total_sum);

    return 0;
}