#define _GNU_SOURCE  // Required for clone()
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sched.h>
#include <unistd.h>

//stack memory for the threads...
#define STACK_SIZE (1024 * 1024) // 1MB stack size

typedef struct thread {
    pid_t tid; //thread ID
    void *stack; //stack pointer to point to its stack memory
} thread_t;

//thread execution 
int thread_function(void *arg) {
    int id = *(int *)arg;
    printf("Thread %d(TID: %d) is running\n", id, gettid());

   
    printf("Performing task....");
    
    //The particular work of specific thread...
    //sleep(2); causing inoredered output
    
    printf("Completed the specific task by thread %d!!!\n", id);
   	
    return 0;
}

// Create a thread using clone()
int thread_create(thread_t *thread, int (*fn)(void *), void *arg) {
	
	//allocating stack memory for the new thread to be created.
	//each thread of the process has its own stack and registers and shares the process code, data among the other threads.
    void *stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                            
    if (stack == MAP_FAILED) {
        perror("stack creation failed...");
        return -1;
    }
	
	//thread stack pointing to the stack allocated to it.
    thread->stack = stack;
    //creating the thread to execute. mapping it to the kernel thread.
    thread->tid = clone(fn, (char *)stack + STACK_SIZE,
                        CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD, arg);
    
    if (thread->tid == -1) {
        perror("thread creation failed...");
        return -1;
    }

    return 0;
}

// waiting for the child threads to get terminated.
int thread_join(thread_t *thread) {
	//waitpid will wait until the child process having pid = thread->tid will not get finsished.
    if (waitpid(thread->tid, NULL, 0) == -1) {
        perror("waitpid failed...");
        return -1;
    }
    
    //cleaning up the memory after the particular work of thread is done.
    munmap(thread->stack, STACK_SIZE);
    
    return 0;
}


int main() {
    thread_t t1, t2;
    //data of threads
    int id1 = 1, id2 = 2;

    // Create threads
    thread_create(&t1, thread_function, &id1);
    thread_create(&t2, thread_function, &id2);

    // Join threads
    thread_join(&t1);
    thread_join(&t2);

    printf("all the threads finished!\n");
    return 0;
}

