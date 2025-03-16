#include <pthread.h>
#include <stdio.h>

// Function that will be executed by thread
// It takes void pointer as argument and returns void pointer
void* threadFunction(void* arg){

    // we will also typecast arg back to integer pointer.
    int* addrOfNum = (int*)arg;
    int num = *addrOfNum;
    printf("You passed number %d\n", num);

    
    printf("Thread ID excuting the threadFunction is : %ld\n", pthread_self());
    return NULL;
}

int main(){

    // pthread_self() returns the thread ID of the calling thread.
    printf("Thread ID excuting the main function before thread creation is : %ld\n", pthread_self());

    // pthread_t is a data type used to uniquely identify a thread (thread indetifer not thread ID)
    pthread_t thread; 
    int num = 5;

    // For creating new thread we have function pthread_create
    // It takes 4 arguments - address of thread, thread attributes, function that thread will excute and arguments to that function.
    // arguments that we are going to pass to threadFunction should be void pointer.
    // So we will first typecast it to void pointer and then pass it to threadFunction. 
    pthread_create(&thread, NULL, threadFunction, (void*)&num); 

    // pthread_join() function is used to wait for the thread to terminate.
    // It takes 2 arguments - thread ID and address of a pointer to store the return value of threadFunction.
    // In short parent thread waits for the child thread to finish.
    pthread_join(thread, NULL);

    printf("Thread ID excuting the main function after thread creation is : %ld\n", pthread_self());

    return 0;
}