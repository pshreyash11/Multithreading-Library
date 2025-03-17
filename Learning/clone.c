//put _GNU_SOU
#define _GNU_SOURCE // required for clone() to be recognized by compiler
#include <stdio.h>
#include <unistd.h> // provides the functions like getpid()
#include <stdlib.h> // for allocating stack memory.
#include <sched.h> // It contains clone() function.
#include <signal.h> // for SIGCHLD



// lets first have size of stack that we are going to give to child process.
#define SIZE_OF_STACK 1024*1024

// WHAT CLONE SYS CALL DOES ? - It creates a new child process, in addition to that it allows us to share memory between parent and child process.
// It is similar to fork() but it allows us to share memory between parent and child process.

// we can control what the new process shares with the parent using flags like:- 
// CLONE_VM - shares memory between parent and child process.
// CLONE_FILES - shares file descriptors between parent and child process.
// CLONE_SIGHAND - shares signal handlers between parent and child process.
// CLONE_FS - Share working directory and file system info

// this is function that will be excuted by child process.
int childFunction(void *arg){
    printf("Hello from inside the child process\n");
    printf("PID of child process is : %d\n", getpid());
    return 0;
}

int main(){

    // Allocate memory for stack of child process.
    char *stack = (char*)malloc(SIZE_OF_STACK);
    if(stack == NULL){
        printf("Unable to allocate memory for stack\n");
        return 1;
    }

    // clone() function will create a new child process
    // arguments taken by clone() - function to excute by child process,
    // pointer to starting of stack. (since stack grows downwards we have to add size of stack to stack which is allocated memory for stack )
    // different different flags present ( see flages above - line 16)
    // arguments for that child function.
    // WHAT DOES CLONE returns ? - It returns the process id of child process.
    // if it returns 0 it means shared memory flag is used (CLONE_VM) it becomes similar to fork system call.
    pid_t child_pid = clone(childFunction, stack + SIZE_OF_STACK, SIGCHLD, NULL);

    if( child_pid == -1 ){
        printf("clone failed");
        free(stack);
        exit(1);
    }

    printf("parents process id = %d , and child process id = %d\n",getpid(), child_pid);

    // we have to stop parent from exiting immidiatly for that we use sleep() function.
    sleep(1);
    free(stack);

    return 0;
}