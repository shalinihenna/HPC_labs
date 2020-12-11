#include <stdio.h>
#include <stdlib.h>

//A = imagen original
//B = imagen resultante
//N = dimensiones (NxN) Matriz Cuadrada
//V = nivel de vecindad

void randomImage(float *A, int N){
    for(int i = 0; i < N*N; i++){
        A[i] = (float)rand()/RAND_MAX;
    }
}

void printImage(float *A, int N){
    int j = 0;
    for(int i = 0; i < N*N; i++){
        printf("%f ",A[i]);
        j++;
        if(j == N){
            printf("\n");
            j = 0;
        }

    }
}

void suma2D_CPU(float *A, float *B, int N, int V){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            B[i * N + j] = 0.0;
            for (int a = i-V; a <= i+V; a++){
                for (int b = j-V; b <= j+V; b++){
                    if(a >= 0 && a < N && b >= 0 && b < N){ 
                        B[i * N + j] = B[i * N + j] + A[a * N + b];
                    }
                }
            }
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
    float *h_B = (float *)malloc(size*sizeof(float));
    //Generación de imagen random
    randomImage(h_A, size);
    printImage(h_A, N);
    printf("\n\n");
    suma2D_CPU(h_A, h_B, 5, 1);
    printImage(h_B, N);

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