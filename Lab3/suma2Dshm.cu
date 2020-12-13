#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

//A = imagen original
//B = imagen resultante
//N = dimensiones (NxN) Matriz Cuadrada
//V = nivel de vecindad
//Bs = tamaño de bloque

__global__ void suma2D_SHMEM(float *A, float *B, int N, int V){
    extern __shared__ float data[];

    int i, j;
    i = blockDim.x * blockIdx.x + threadIdx.x; //horizontal
    j = blockDim.y * blockIdx.y + threadIdx.y; //vertical
    
    int tid = i * N + j;

    data[tid] = 0.0;

    for (int a = i-V; a <= i+V; a++){
        for (int b = j-V; b <= j+V; b++){
            if(a >= 0 && a < N && b >= 0 && b < N){
                data[i * N + j] = data[i * N + j] + A[a * N + b];
            }
        }
    }

    __syncthreads();
    B[tid] = data[tid];
}

void suma2D_CPU(float *A, float *B, int N, int V){
    //TODO: Quitar el elemento i,j dentro de la suma o preguntar
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

__host__ int main(void){
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
    printf("Imagen Original:\n ");
    printImage(h_A, N);
    printf("\n\n");

    //Se empieza a medir el tiempo en GPU
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start,0);

    //Pedir memoria en device
    float *d_A, *d_B;
    cudaMalloc((void **) &d_A, size*sizeof(float));
    cudaMalloc((void **) &d_B, size*sizeof(float));

    //Copia desde Host a Device
    cudaMemcpy(d_A, h_A, size*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size*sizeof(float), cudaMemcpyHostToDevice);

    //Llamado a la función de suma en GPU
    dim3 blockSize = dim3(N/Bs, N/Bs);
    dim3 gridSize = dim3(Bs,Bs);
    suma2D_SHMEM<<<gridSize,blockSize, size>>>(d_A, d_B, N, V);

    //Copia desde Device a Host
    cudaMemcpy(h_B, d_B, size*sizeof(float), cudaMemcpyDeviceToHost);

    //Se termine de medir el tiempo en GPU
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    float elapsedTime;
    cudaEventElapsedTime( &elapsedTime, start, stop);

    //Se imprime por consola la imagen nueva y el tiempo de ejecución en GPU
    printf("Imagen Resultante en GPU:\n ");
    printImage(h_B, N);
    printf("Tiempo de Ejecucion GPU: %3.lf ms.\n", elapsedTime);
    printf("\n\n");

    //Se empieza a medir tiempo en CPU
    double time_spent = 0.0;
    clock_t begin = clock(); 

    //Llamado a la función de suma en CPU
    suma2D_CPU(h_A, h_B, N, V);
    printf("Imagen Resultante en CPU:\n ");
    printImage(h_B, N);

    //Se termina de medir tiempo en CPU
    clock_t end = clock(); 
    time_spent += (double)(end-begin)/CLOCKS_PER_SEC;
    printf("Tiempo de Ejecucion CPU: %f seg.\n", time_spent);

    //Se libera memoria solicitada
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(d_A);
    cudaFree(d_B);
    free(h_A);
    free(h_B);

    exit(0);

}