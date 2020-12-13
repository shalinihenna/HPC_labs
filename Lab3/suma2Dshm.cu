#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>


//Ejemplo compilacion: nvcc suma2Dshm.cu -o suma2Dshm
//Ejemplo ejecucion: ./suma2Dshm -N 5 -B 1 -V 1

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
        //A[i] = 1;
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

__host__ int main(int argc, char **argv){
    
    int N = 0;
    int Bs = 0;
    int V = 0;
    int c1;
    
    while((c1 =  getopt(argc, argv,"N:B:V:")) != -1){
        switch(c1){
            case 'N':
                if(atof(optarg) < 1){
                    printf("El valor ingresado debe ser mayor que 0\n");
                    c1 = -1;    
                    break;
                }
                else N = atof(optarg);
                break;
            case 'B':
                if(atof(optarg) < 1){
                    printf("El valor ingresado debe ser mayor que 0\n");
                    c1 = -1;    
                    break;
                }
                else Bs = atof(optarg);
                break;  
            case 'V':
                if(atof(optarg) < 1){
                    printf("El valor ingresado debe ser mayor que 0\n");
                    c1 = -1;    
                    break;
                }
                else V = atof(optarg);
                break;  
            case '?':
                if(optopt == 'N' || optopt == 'B' || optopt == 'V')
                    fprintf(stderr, "Option -%c requeries an argument.\n",optopt);
                else if(isprint(optopt))
                    fprintf(stderr,"Unknown option -%c.\n",optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n",optopt);
                return 1;
            default:
                abort();
        }
    }

    int size = N*N;

    //Pedir memoria en host
    float *h_A = (float *)malloc(size*sizeof(float));
    float *h_B = (float *)malloc(size*sizeof(float));

    //Generación de imagen random
    randomImage(h_A, N);
    printf("\n\nImagen original\n");
    printImage(h_A, N);

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
    suma2D_SHMEM<<<gridSize, blockSize, size>>>(d_A, d_B, N, V);

    //Copia desde Device a Host
    cudaMemcpy(h_B, d_B, size*sizeof(float), cudaMemcpyDeviceToHost);
    printf("\n\nImagen sumada desde GPU con memoria compartida\n");
    printImage(h_B, N);

    //Llamado a la función de suma en CPU
    suma2D_CPU(h_A, h_B, N, V);
    printf("\n\nImagen sumada desde CPU\n");
    printImage(h_B, N);
    exit(0);

}