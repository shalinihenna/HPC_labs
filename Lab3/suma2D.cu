#include <stdio.h>
#include <stdlib.h>

//A = imagen original
//B = imagen resultante
//N = dimensiones (NxN) Matriz Cuadrada
//V = nivel de vecindad
//Bs = tamaño de bloque

__global__ void suma2D(){

}

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

    dim3 blockSize = dim3(N/Bs, N/Bs);
    dim3 gridSize = dim3(Bs,Bs);

    suma2D<<gridSize,blockSize>>(A,B,N,V);

    
}


/*COSAS POR HACER
- suma2D
- suma2Dshmem
- suma2D_CPU
- No sé si hay que agregar __host__ a las funciones que no son __global__
*/