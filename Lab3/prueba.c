#include <stdio.h>
#include <stdlib.h>

//A = imagen original
//B = imagen resultante
//N = dimensiones (NxN) Matriz Cuadrada
//V = nivel de vecindad
//#define N 100
#define V 1

void randomImage(float **A, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            A[i][j] = (float)rand()/RAND_MAX;
        }
    }
}

void printImage(float **A, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%f ",A[i][j]); 
        }
        printf("\n");
    }
}

int main(void){

    int N = 5;
    //Generación de imagen random
    float **A = (float **) malloc(N*sizeof(float*));
    for(int i = 0; i < N; i++) A[i] = (float *)malloc(N * sizeof(float));
    randomImage(A, N);
    printImage(A,N);

    /*dim3 blockSize = dim3(N/B, N/B);
    dim3 gridSize = dim3(B,B);

    suma2D<<gridSize,blockSize>>(A,B,N,V);*/


}