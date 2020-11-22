//
//  Autor: Joaquín Ignacio Jara Marín - Shalini Henna Ramchandani Moorjimal
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
                else h = atof(optarg);
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
    float * lista;   
    lista = readNumbers(i,N);

    printf("i: %s\n", i);
    printf("o: %s\n", o);
    printf("N: %i\n", N);
    printf("d: %i\n", d);
    printf("l: %i\n", l);
    printf("h: %i\n", h);

    omp_set_num_threads(h);
    #pragma omp parallel
    #pragma omp single nowait
    divideYOrdenaras(lista, N, l);
    
    #pragma omp critical
    printf("Lista final final\n");
    for (int i = 0; i < N; i++)
    {
        printf("%f\n",lista[i]);
    }
    printf("\n");
    return 0;
}