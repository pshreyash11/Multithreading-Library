#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>  

#define MAX_THREADS 100000  // Arbitrary large number to test the limit

void *thread_func(void *arg) {
    sleep(1000);  // Keep threads alive to avoid immediate termination
    return NULL;
}

int main() {
    pthread_t threads[MAX_THREADS];
    int count = 0;

    for (int i = 0; i < MAX_THREADS; i++) {
        int res = pthread_create(&threads[i], NULL, thread_func, NULL);
        if (res != 0) {
            printf("\n❌ Failed to create thread %d. Error: %d (%s)\n", i, errno, strerror(errno));
            break;
        }
        count++;
        printf("✅ Created thread %d\n", i);
    }

    printf("\n📌 Maximum kernel threads created: %d\n", count);
    
    // Clean up threads to avoid memory leaks
    for (int i = 0; i < count; i++) {
        pthread_cancel(threads[i]); // Cancel threads
        pthread_join(threads[i], NULL); // Wait for cleanup
    }

    return 0;
}
