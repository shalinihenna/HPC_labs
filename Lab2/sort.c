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
#include <omp.h>

#include "funciones.h"


int main(int argc, char **argv)
{   
    char* i;
    char* o;
    int N = 0;
    int d = 0;
    int l = 0;
    int h = 0;
    int c1;

    while((c1 =  getopt(argc, argv,"i:o:N:d:l:h:")) != -1){
        switch(c1){
            case 'i':
                if(strcmp(optarg, "") == 0){
                    printf("Debe ingresar un nombre del archivo binario con los valores de entrada\n");
                    c1 = -1;
                    break;
                }
                else i = optarg;
                break;
            case 'o':
                if(strcmp(optarg, "") == 0){
                    printf("Debe ingresar un nombre para el archivo binario de salida\n");
                    c1 = -1;
                    break;
                }
                else o = optarg;
                break;
            case 'N':
                if(atof(optarg) < 1){
                    printf("El valor ingresado debe ser mayor que 0\n");
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
            case 'l':
                if(atof(optarg) < 0 ){
                    printf("El valor de los niveles de recursividad debe ser mayor o igual que 0\n");
                    c1 = -1;    
                    break;
                }
                else l = atof(optarg);
                break;
            case 'h':
                if(atof(optarg) < 1 ){
                    printf("El valor de hebras debe ser mayor o igual que 1\n");
                    c1 = -1;    
                    break;
                }
                else l = atof(optarg);
                break;  
            case '?':
                if(optopt == 'i' || optopt == 'o' || optopt == 'N' || optopt == 'd' || optopt == 'l' || optopt == 'h')
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
    
    // Lectura de archivos
    //float * lista;   
    //lista = readNumbers(i,N);

    printf("i: %s\n", i);
    printf("o: %s\n", o);
    printf("N: %i\n", N);
    printf("d: %i\n", d);
    printf("l: %i\n", l);
    printf("h: %i\n", h);
    //Etapa SIMD
    /* double time_spent = 0.0;
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
        //Imprimir secuencia final
        int k;
        for (k = 0; k < N; k++)
        {  
            printf("%f\n", lista[k]);
        }
    }

    //Escribir archivo de salida
    writeNumbers(o, lista, N);
 */
    
    return 0;
}