#include <stdio.h>
#include <stdlib.h>

//A = imagen original
//B = imagen resultante
//N = dimensiones (NxN) Matriz Cuadrada
//V = nivel de vecindad
//Bs = tamaño de bloque

__global__ void suma2D(float *A, float *B, int N, int V){

}

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

int main(void){
    //Variables
    //TODO: GetOpt
    int N = 5;
    int size = N*N;
    int V = 1;
    int Bs = 1;

    //Pedir memoria en host
    float *h_A = (float *)malloc(size*sizeof(float));
    float *h_B = (float *)malloc(size*sizeof(float));

    //Generación de imagen random
    randomImage(h_A, N);
    printImage(h_A, N);

    //Pedir memoria en device
    float *d_A, *d_B;
    cudaMalloc((void **) &d_A, size*sizeof(float));
    cudaMalloc((void **) &d_B, size*sizeof(float));

    //Copia desde Host a Device
    cudaMemcpy(d_A, h_A, size*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size*sizeof(float), cudaMemcpyHostToDevice);

    //Llamado a la función de suma
    dim3 blockSize = dim3(N/Bs, N/Bs);
    dim3 gridSize = dim3(Bs,Bs);
    suma2D<<gridSize,blockSize>>(d_A, d_B, N, V);

    //Copia desde Device a Host
    cudaMemcpy(&h_B, d_B, size*sizeof(float), cudaMemcpyDeviceToHost);
    printImage(h_B, N);
    
}


/*COSAS POR HACER
- suma2D
- suma2Dshmem
- suma2D_CPU
- No sé si hay que agregar __host__ a las funciones que no son __global__
*/