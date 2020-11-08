//
//  Autor: Joaquín Ignacio Jara Marín - Shalini Ramchandani Henna Moorjimal
//  Rut: 19.739.353-k - 19.307.417-0
//  Curso: HPC
//  Profesor: Fernando Rannou
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mmintrin.h> /* MMX instrinsics __m64 integer type */
#include <xmmintrin.h> /* SSE __m128 float */
#include <ctype.h>
#include <unistd.h>
#include <time.h> 

#include "funciones.h"


int main(int argc, char **argv)
{   
    char* i;
    char* o;
    int N = 0;
    int d = 0;
    int c1;

    while((c1 =  getopt(argc, argv,"i:o:N:d:")) != -1){
        switch(c1){
            case 'i':
                if(strcmp(optarg, "") == 0){
                    printf("Debe ingresar un nombre del archivo binario con los valores de entrada");
                    c1 = -1;
                    break;
                }
                else i = optarg;
                break;
            case 'o':
                if(strcmp(optarg, "") == 0){
                    printf("Debe ingresar un nombre para el archivo binario de salida");
                    c1 = -1;
                    break;
                }
                else o = optarg;
                break;
            case 'N':
                if(atof(optarg) < 1){
                    printf("El valor ingresado debe ser mayor que 0");
                    c1 = -1;    
                    break;
                }
                else N = atof(optarg);
                break;
            case 'd':
                if(atof(optarg) != 0 && atof(optarg) != 1){
                    printf("El valor de debug debe ser 0 o 1");
                    c1 = -1;    
                    break;
                }
                else d = atof(optarg);
                break;  
            case '?':
                if(optopt == 'i' || optopt == 'o' || optopt == 'N' || optopt == 'd')
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
    int cantListas = N/16;
    float ** matrizListas = createMatriz(cantListas);

    
    // Lectura de archivos
    float * lista;   
    float * listaDos;
    lista = readNumbers(i,N);
    listaDos = readNumbers(i,N);

    //Etapa SIMD
    double time_spent = 0.0;
    clock_t begin = clock(); //Empieza el tiempo
    int j = 0;
    while(j < N){

        //Loading Registers
        __m128 r1,r2,r3,r4;
        float R1[4] __attribute__((aligned(16))) = {lista[j], lista[j+1], lista[j+2], lista[j+3]};
        float R2[4] __attribute__((aligned(16))) = {lista[j+4], lista[j+5], lista[j+6], lista[j+7]};
        float R3[4] __attribute__((aligned(16))) = {lista[j+8], lista[j+9], lista[j+10],lista[j+11]};
        float R4[4] __attribute__((aligned(16))) = {lista[j+12],lista[j+13],lista[j+14],lista[j+15]};

        r1 = _mm_load_ps(R1);
        r2 = _mm_load_ps(R2);
        r3 = _mm_load_ps(R3);
        r4 = _mm_load_ps(R4);

        orderInRegister(&r1, &r2, &r3, &r4);
        mergeSIMD(&r1, &r2, &r3, &r4);

        _mm_store_ps(R1,r1);
        _mm_store_ps(R2,r2);
        _mm_store_ps(R3,r3);
        _mm_store_ps(R4,r4);
        
        storeList(matrizListas[j/16], R1, R2, R3, R4);
        j = j + 16;
    }
        
    //Ordenamiento
    MWMS(matrizListas, lista, N);
    clock_t end = clock(); //Termina el tiempo
    time_spent += (double)(end-begin)/CLOCKS_PER_SEC;

    if (d == 1)
    {   
        printf("\n\nMerge SIMD:\n");
        //Imprimir secuencia final
        int k;
        for (k = 0; k < N; k++)
        {  
            printf("%f, ", lista[k]);
        }
        printf("\n");
        printf("\nTiempo de ejecucion simdsort: %f seg.\n",time_spent);
    }

    //Escribir archivo de salida
    writeNumbers(o, lista, N);

    //liberacion de memoria
    freeMemory(lista, matrizListas, cantListas);

    //Algoritmo de ordenamiento secuencial: Selection Sort.
    double time_spent2 = 0.0;
    clock_t begin2 = clock(); //Empieza el tiempo
    SelectionSort(listaDos,N);
    clock_t end2 = clock(); //Termina el tiempo
    time_spent2 += (double)(end2-begin2)/CLOCKS_PER_SEC;

    if (d == 1)
    {    
        printf("\n\nSelection Sort:\n");
        //Imprimir secuencia final
        int k;
        for (k = 0; k < N; k++)
        {  
            printf("%f, ", listaDos[k]);
        }
        printf("\n");
        printf("\nTiempo de ejecucion selection sort: %f seg.\n",time_spent2);
    }
    
    return 0;
}