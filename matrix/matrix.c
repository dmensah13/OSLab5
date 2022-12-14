//Received Help from Demetria Mack
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int MAX;

int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    char size[5];
    printf("Enter the size of matrix: ");
    fgets(size, 5, stdin);
    MAX = atoi(size);
    
  struct args_t{
      int (*matResult)[MAX];
      int (*matA)[MAX];
      int (*matB)[MAX];
      int row;
    };
  void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* computeSum(void* args) { 
  struct args_t* arg = (struct args_t*) args;
  int r = arg->row*MAX;
  for(int i = 0; i<MAX; i++){
    arg->matResult[0][r+i] = arg->matA[0][r+i] + arg->matB[0][r+i];
  }
}

void* computeDiff(void* args) { 
  struct args_t* arg = (struct args_t*) args;
  int r = arg->row*MAX;
  for(int i = 0; i<MAX; i++){
    arg->matResult[0][r+i] = arg->matA[0][r+i] - arg->matB[0][r+i];
  }
}

void* computeProduct(void* args) { 
   struct args_t* arg = (struct args_t*) args;
  int r = arg->row*MAX;
  
  for (int j = 0; j < MAX; j++){
    int dot_product = 0;
    for(int i = 0; i<MAX; i++){
      int c = i*MAX;
      dot_product = dot_product + arg->matA[0][r+i] * arg->matB[0][c+j];
    }
    arg->matResult[0][r+j] = dot_product;
 }
}
  
    int matA[MAX][MAX]; 
    int matB[MAX][MAX]; 

    int matSumResult[MAX][MAX];
    int matDiffResult[MAX][MAX]; 
    int matProductResult[MAX][MAX];

    fillMatrix(matA);
    fillMatrix(matB);
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);    
    pthread_t threads[MAX];
    struct args_t args[MAX];
    for (int i =0; i<MAX; i++){
      args[i].matResult = matSumResult;
      args[i].matA = matA;
      args[i].matB = matB;
      args[i].row = i;
      int err = pthread_create(&threads[i], NULL, &computeSum, (void *) &args[i]);
      if (err)
      {
        printf("Thread creation failed");
        exit(1);
       }
     
    }
    for (int i =0; i<MAX; i++){
      pthread_join(threads[i], NULL);
    }
  
    for (int i =0; i<MAX; i++){
      args[i].matResult = matDiffResult;
      args[i].matA = matA;
      args[i].matB = matB;
      args[i].row = i;
      int err = pthread_create(&threads[i], NULL, &computeDiff, (void *) &args[i]);
      if (err){
        printf("Thread creation failed");
        exit(1);
       }
     
    }
    for (int i =0; i<MAX; i++){
      pthread_join(threads[i], NULL);
    }
    for (int i =0; i<MAX; i++){
      args[i].matResult = matProductResult;
      args[i].matA = matA;
      args[i].matB = matB;
      args[i].row = i;
      int err = pthread_create(&threads[i], NULL, &computeProduct, (void *) &args[i]);
      if (err)
      {
        printf("Thread creation failed");
        exit(1);
       }
     
    }
    for (int i =0; i<MAX; i++){
      pthread_join(threads[i], NULL);
    }
    
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;

}
