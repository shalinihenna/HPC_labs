#include <stdio.h>

__global__ void suma2D_SHMEM(){
    
}

__host__ int main(void){

    dim3 blockSize = dim3(N/B, N/B);
    dim3 gridSize = dim3(B,B);

    suma2D_SHMEM<<gridSize,blockSize>>(A,B,N,V);


}