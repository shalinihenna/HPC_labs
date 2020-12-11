#include <stdio.h>

//A = imagen original
//B = imagen resultante
//N = dimensiones (NxN) Matriz Cuadrada
//V = nivel de vecindad

__global__ void suma2D(){

}

__host__ void randomImage(float **A, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            A[i][j] = (float)rand()/RAND_MAX;
        }
    }
}

__host__ void printImage(float **A, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%f ",A[i][j]); 
        }
        printf("\n");
    }
}

__host__ int main(void){
    //Variables
    //TODO: GetOpt
    int N = 5;
    int V = 1;

    //Generación de imagen random
    float **A = (float **) malloc(N*sizeof(float*));
    for(int i = 0; i < N; i++) A[i] = (float *)malloc(N * sizeof(float));
    randomImage(A, N);
    printImage(A, N);

    dim3 blockSize = dim3(N/B, N/B);
    dim3 gridSize = dim3(B,B);

    suma2D<<gridSize,blockSize>>(A,B,N,V);


}