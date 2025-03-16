#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10

typedef struct{
    int start;
    int end;
    int result;
}ThreadData;

void* calculateFactorialForPart(void* arg){
    ThreadData* data = (ThreadData*)arg;
    int start = data->start;
    int end = data->end;
    data->result = 1;
    for(int i=start;i<=end;i++){
        data->result *= i;
    }
    pthread_exit(NULL);
}

unsigned long long calculateFactorial(int num){
    if(num == 0 || num == 1){
        return 1;
    }

    pthread_t threads[NUM_THREADS];
    ThreadData data_of_threads[NUM_THREADS];
    
    // Lets find out how much part each thread will get to calculate.
    int chunkSize = num / NUM_THREADS;
    
    // Sometimes it can happen like num is not completely divided by NUM_THREADS
    // In this case we will give some additional part to first few threads.
    // For e.g if num=20 and NUM_THREADS=6 then first 2 threads will get 4 elements and rest of the threads will get 3 elements.
    // So lets calculate how many threads will get additional part. for that we will need remainder.
    int remainder = num % NUM_THREADS;

    int start = 1;

    for(int i=0;i<NUM_THREADS;i++){
        data_of_threads[i].start = start;
        
        int extraPart = (i < remainder ? 1 : 0); // extra part mentioned above
        data_of_threads[i].end = start + chunkSize - 1 + extraPart;
    
        pthread_create(&threads[i], NULL, (void*)calculateFactorialForPart, (void*)&data_of_threads[i]);
        start = data_of_threads[i].end + 1;
    }

    unsigned long long result = 1;
    for(int i=0;i<NUM_THREADS;i++){
        pthread_join(threads[i], NULL);
        result *= data_of_threads[i].result;
    }
    return result;

}

int main(){

    int num;
    printf("Enter the number to find factorial : ");
    scanf("%d", &num);

    if(num < 0){
        printf("Factorial of negative number is not possible\n");
        return 0;
    }

    unsigned long long result = calculateFactorial(num);
    printf("Factorial of %d is : %llu\n", num, result);

    return 0;
}