//
//  Autor: Joaquín Ignacio Jara Marín - Shalini Henna Ramchandani Moorjimal
//  Rut: 19.739.353-k - 19.307.417-0
//  Curso: HPC
//  Profesor: Fernando Rannou
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//Funcion que genera una nueva imagen a partir de una vecindad indicada utilizando memoria compartida
//Entrada:  -Puntero de float que representa la imagen original
//          -Puntero de float que representa la imagen resultante
//          -N, Entero que indica el largo de una matriz de NxN
//          -V, Entero que indica el tamaño de la vecindad
__global__ void suma2D_SHMEM(float *A, float *B, int N, int V){
    __shared__ float data[1024];
    
    int i, j, k;
    i = blockDim.x * blockIdx.x + threadIdx.x;
    j = blockDim.y * blockIdx.y + threadIdx.y;
    k = i * N + j;
    
    data[i] = 0.0;

    for (int a = i-V; a <= i+V; a++){
        for (int b = j-V; b <= j+V; b++){
            if(a >= 0 && a < N && b >= 0 && b < N){
                data[i] = data[i] + A[a * N + b];
            }
        }
    }
    B[k] = data[i];
}

//Funcion que genera una nueva imagen a partir de una vecindad indicada en CPU
//Entrada:  -Puntero de float que representa la imagen original
//          -Puntero de float que representa la imagen resultante
//          -N, Entero que indica el largo de una matriz de NxN
//          -V, Entero que indica el tamaño de la vecindad
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

//Funcion que genera una nueva imagen aleatoria de NxN
//Entrada:  -Puntero de float que genera una imagen con pixeles random
//          -N, Entero que indica el largo de una imagen de NxN
void randomImage(float *A, int N){
    for(int i = 0; i < N*N; i++){
        A[i] = (float)rand()/RAND_MAX;
    }
}

//Funcion que imprime los valores de una imagen
//Entrada:  -Puntero de float que representa una imagen
//          -N, Entero que indica el largo de una imagen de NxN
void printImage(float *A, int N){
    int j = 0;
    for(int i = 0; i < N*N; i++){
        printf("%f\n",A[i]);
        j++;
    }
}

int main(int argc, char **argv){
    
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
    float *h_C = (float *)malloc(size*sizeof(float));

    //Generación de imagen random
    randomImage(h_A, N);
    printf("Imagen Original:\n ");
    printImage(h_A, N);
    printf("\n");

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
    dim3 blockSize = dim3(Bs, Bs);
    dim3 gridSize = dim3(N/Bs,N/Bs);
    suma2D_SHMEM<<<gridSize, blockSize>>>(d_A, d_B, N, V);

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

    //Se empieza a medir tiempo en CPU
    double time_spent = 0.0;
    clock_t begin = clock(); 

    //Llamado a la función de suma en CPU
    suma2D_CPU(h_A, h_C, N, V);
    printf("Imagen Resultante en CPU:\n ");
    printImage(h_C, N);

    //Se termina de medir tiempo en CPU
    clock_t end = clock(); 
    time_spent += (double)(end-begin)/CLOCKS_PER_SEC;

    printf("Tiempo de Ejecucion GPU: %f seg.\n", elapsedTime/1000);
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