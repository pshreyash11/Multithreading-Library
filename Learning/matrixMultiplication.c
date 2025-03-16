#include <stdio.h>
#include <pthread.h>

// For succesufull matrix multiplication, matrix should be in form of A * B and B * C then result will be A * C
#define A 3
#define B 3
#define C 3

int X[A][B] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
int Y[B][C] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
int Z[A][C]; // Resultant matrix

// Approach :- Each thread will calculate one row of resultant matrix.
// we could have taken each thread for each element but in the case of big matrix we will have to create large number of threads.


void* matrixMultiplication(void* arg){
    int data = *(int*)arg;
    int row = data;

    // we will complete one row of resultant matrix. with below loop
    for(int i=0;i<C;i++){
 
        Z[row][i] = 0;

        // when we are calculating one element of resultant matrix, we are multiplying one row of X matrix with one column of Y matrix.
        // If you notice carefully, completing one element of fixed row of resultant matrix,
        // rowValue of X matrix is fixed and colValue of Y matrix is fixed.
        // only changing element is colValue of X i.e rowValue of Y matrix. thats why below loop
        for(int j=0;j<B;j++){
            Z[row][i] += X[row][j] * Y[j][i];
        }
    }

    pthread_exit(NULL);
}

int main(){

    pthread_t threads[A]; // A threads for A rows of resultant matrix
    int thread_data[A]; // A thread_data for A threads (This is same thing like we did in additionOfArray.c)

    for(int i=0;i<A;i++){
        thread_data[i] = i;
        pthread_create(&threads[i], NULL, (void*)matrixMultiplication, (void*)&thread_data[i]);
    }

    // Now we will just wait for threads to finish their excution.
    for(int i=0;i<A;i++){
        pthread_join(threads[i], NULL);
    }

    printf("Resultant matrix is : \n");
    for(int i=0;i<A;i++){
        for(int j=0;j<C;j++){
            printf("%d ", Z[i][j]);
        }
        printf("\n");
    }

    return 0;
}