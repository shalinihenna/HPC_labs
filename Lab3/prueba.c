#include <stdio.h>
#include <stdlib.h>

//A = imagen original
//B = imagen resultante
//N = dimensiones (NxN) Matriz Cuadrada
//V = nivel de vecindad

void randomImage(float *A, int size){
    for(int i = 0; i < size; i++){
        A[i] = (float)rand()/RAND_MAX;
    }
}

void printImage(float *A, int size, int N){
    int j = 0;
    for(int i = 0; i < size; i++){
        printf("%f ",A[i]);
        j++;
        if(j == N){
            printf("\n");
            j = 0;
        }

    }
}

int main(void){
    //Variables
    //TODO: GetOpt
    int N = 5;
    int size = N*N;
    int V = 1;

    //Pedir memoria en host
    float *h_A = (float *)malloc(size*sizeof(float));
    //Generación de imagen random
    randomImage(h_A, size);
    printImage(h_A, size, N);

    /*dim3 blockSize = dim3(N/B, N/B);
    dim3 gridSize = dim3(B,B);

    suma2D<<gridSize,blockSize>>(A,B,N,V);*/


}
/* RECORRER LA MATRIZ COMO ARREGLO
 for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            h_A[i*N+j] = sin(i);
            h_B[i*N+j] = cos(j);
        }
    }*/