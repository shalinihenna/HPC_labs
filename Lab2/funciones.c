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

//Funcion que crea y asigna memoria una lista de float de tamaño N
//Entrada:  -N, entero que indica el largo de la lista a crear
//Salida:   -Puntero de float para la representacion de la lista
float * createList(int N){
    float * lista = (float*)malloc(sizeof(float) * N);
    return lista;
}

//Funcion que crea y asigna memoria a una lista de listas de largo 16
//Entrada:  -N, Entero que indica la cantidad de listas
//Salida:   -Puntero a puntero de float para la representacion de una lista de listas con memoria reservada
float ** createMatriz(int N){
    int i;
    float ** matrix = (float**)malloc(sizeof(float*) * N);

    for (i = 0; i < N; i++)
    {
        matrix[i] = (float*)malloc(sizeof(float) * 16);
    }

    return matrix;
}

//Funcion que lee de un archivo una lista de numeros flotantes
//Entrada:  -Cadena de char que indica el nombre del archivo a leer
//          -N, Entero que indica el largo de la lista
//Salida:   -Puntero de float para la representacion de una lista con memoria reservada
float * readNumbers(char * name, int N){
    int i;
    float * listaSalida = createList(N);

    FILE *ptr;
    ptr = fopen(name,"rb");

    for (i = 0; i < N; i++)
    {
        float a;
        fread(&a,sizeof(float),1,ptr);
        listaSalida[i] = a;
    }

    fclose(ptr);
    return listaSalida;
}

//Funcion que escribe una lista de flotantes de largo N, en un archivo
//Entrada:  -Cadena de char que contiene el nombre del archivo de salida
//          -Puntero de float que representa la lista ordenada de numeros
//          -N, Entero que indica el total de elementos de la lista
void writeNumbers(char * name, float * lista, int N){
    FILE * ptr = fopen(name, "w+");
    int x;
    for (x= 0; x < N; x++)
    {
        fwrite(&lista[x], sizeof(float), 1, ptr);
    }
    fclose(ptr);
}

//Funcion que imprime los 4 valores de un registro SIMD
//Entrada:  -r1, Registro SIMD con 4 valores flotantes
void printRegister(__m128 r1){
    float R1[4] __attribute__((aligned(16)));
    _mm_store_ps(R1,r1);
    printf("%f %f %f %f\n",R1[0],R1[1],R1[2],R1[3]);
}

//Funcion que imprime los 16 valores de 4 registros SIMD ordenadamente
//Entrada:  -r1, Registro SIMD con 4 valores flotantes
//          -r2, Registro SIMD con 4 valores flotantes
//          -r3, Registro SIMD con 4 valores flotantes
//          -r4, Registro SIMD con 4 valores flotantes
void printRegisters(__m128 r1, __m128 r2, __m128 r3, __m128 r4){
    printRegister(r1);
    printRegister(r2);
    printRegister(r3);
    printRegister(r4);
    printf("\n");
}

//Funcion que implementa logica del algoritmo de ordenamiento Sorting Network
//Entrada:  -r1, Referencia registro SIMD con 4 valores flotantes
//          -r2, Referencia registro SIMD con 4 valores flotantes
//          -r3, Referencia registro SIMD con 4 valores flotantes
//          -r4, Referencia registro SIMD con 4 valores flotantes
void sortingNetwork(__m128* r1, __m128* r2, __m128* r3, __m128* r4){
    __m128 r13min, r13max, r24min, r24max, r12max, r34min;
    r13min = _mm_min_ps(*r1,*r3);
    r13max = _mm_max_ps(*r1,*r3);

    r24min = _mm_min_ps(*r2,*r4);
    r24max = _mm_max_ps(*r2,*r4);

    *r1 = _mm_min_ps(r13min,r24min); //r12min
    r12max = _mm_max_ps(r13min,r24min);

    r34min = _mm_min_ps(r13max,r24max);
    *r4 = _mm_max_ps(r13max,r24max); //r34max

    *r2 = _mm_min_ps(r12max,r34min); //r23min
    *r3 = _mm_max_ps(r12max,r34min); //r23max 
}

//Funcion que transpone los valores de los registros SIMD de entrada, donde cada
//  registro representa una fila, y las mismas posiciones de cada registro las columnas
//Entrada:  -r1, Referencia registro SIMD con 4 valores flotantes
//          -r2, Referencia registro SIMD con 4 valores flotantes
//          -r3, Referencia registro SIMD con 4 valores flotantes
//          -r4, Referencia registro SIMD con 4 valores flotantes
void transpose(__m128* r1, __m128* r2, __m128* r3, __m128* r4){
    __m128 rAux, r1Mid, r2Mid, r3Mid, r4Mid;

    //-----------Paso 1-----------
    rAux = _mm_shuffle_ps(*r1,*r3,_MM_SHUFFLE(1,0,1,0));
    r1Mid = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    rAux = _mm_shuffle_ps(*r1,*r3,_MM_SHUFFLE(3,2,3,2));
    r2Mid = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    rAux = _mm_shuffle_ps(*r2,*r4,_MM_SHUFFLE(1,0,1,0));
    r3Mid = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    rAux = _mm_shuffle_ps(*r2,*r4,_MM_SHUFFLE(3,2,3,2));
    r4Mid = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    //-----------Paso 2-----------
    rAux = _mm_shuffle_ps(r1Mid,r3Mid,_MM_SHUFFLE(1,0,1,0));
    *r1 = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));
    
    rAux = _mm_shuffle_ps(r1Mid,r3Mid,_MM_SHUFFLE(3,2,3,2));
    *r2 = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));
    
    rAux = _mm_shuffle_ps(r2Mid,r4Mid,_MM_SHUFFLE(1,0,1,0));
    *r3 = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    rAux = _mm_shuffle_ps(r2Mid,r4Mid,_MM_SHUFFLE(3,2,3,2));
    *r4 = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));
}

//Funcion que implementa la logica del algoritmo bitonic merge network
//Entrada:  -r1, Referencia registro SIMD con 4 valores flotantes ordenados ascendentemente
//          -r2, Referencia registro SIMD con 4 valores flotantes ordenados ascendentemente
void bmn(__m128* r1, __m128* r2){
    __m128 r1Aux, r2Aux;

    *r2 = _mm_shuffle_ps(*r2,*r2,_MM_SHUFFLE(0,1,2,3)); //Se invierte

    //-----------Primera etapa-----------
    r1Aux = _mm_shuffle_ps(*r1,*r1,_MM_SHUFFLE(3,1,2,0));
    r2Aux = _mm_shuffle_ps(*r2,*r2,_MM_SHUFFLE(3,1,2,0));

    *r1 = _mm_min_ps(r1Aux, r2Aux);
    *r2 = _mm_max_ps(r1Aux, r2Aux);

    //-----------Segunda etapa-----------
    r1Aux = _mm_shuffle_ps(*r1,*r2,_MM_SHUFFLE(2,0,2,0));
    r2Aux = _mm_shuffle_ps(*r1,*r2,_MM_SHUFFLE(3,1,3,1));

    r1Aux = _mm_shuffle_ps(r1Aux,r1Aux,_MM_SHUFFLE(3,1,2,0));
    r2Aux = _mm_shuffle_ps(r2Aux,r2Aux,_MM_SHUFFLE(3,1,2,0));

    *r1 = _mm_min_ps(r1Aux, r2Aux);
    *r2 = _mm_max_ps(r1Aux, r2Aux);

    //-----------Tercera etapa-----------
    r1Aux = _mm_shuffle_ps(*r1,*r2,_MM_SHUFFLE(1,0,1,0));
    r2Aux = _mm_shuffle_ps(*r1,*r2,_MM_SHUFFLE(3,2,3,2));

    r1Aux = _mm_shuffle_ps(r1Aux,r1Aux,_MM_SHUFFLE(3,1,2,0));
    r2Aux = _mm_shuffle_ps(r2Aux,r2Aux,_MM_SHUFFLE(3,1,2,0));

    *r1 = _mm_min_ps(r1Aux, r2Aux);
    *r2 = _mm_max_ps(r1Aux, r2Aux);

    //-----------Ordenamiento----------
    r1Aux = _mm_shuffle_ps(*r1,*r2,_MM_SHUFFLE(1,0,1,0));
    r2Aux = _mm_shuffle_ps(*r1,*r2,_MM_SHUFFLE(3,2,3,2));

    *r1 = _mm_shuffle_ps(r1Aux,r1Aux,_MM_SHUFFLE(3,1,2,0));
    *r2 = _mm_shuffle_ps(r2Aux,r2Aux,_MM_SHUFFLE(3,1,2,0));
}

//Funcion que implementa la logica del algoritmo Merge SIMD de secuencias
//Entrada:  -r1, Referencia registro SIMD con 4 valores flotantes ordenados ascendentemente
//          -r2, Referencia registro SIMD con 4 valores flotantes ordenados ascendentemente
//          -r3, Referencia registro SIMD con 4 valores flotantes ordenados ascendentemente
//          -r4, Referencia registro SIMD con 4 valores flotantes ordenados ascendentemente
void mergeSIMD(__m128* r1, __m128* r2, __m128* r3, __m128* r4){

    float aux2[4] __attribute__((aligned(16)));
    float aux4[4] __attribute__((aligned(16)));

    __m128 o1, o2;

    bmn(r1,r3);

    _mm_store_ps(aux2,*r2);
    _mm_store_ps(aux4,*r4);

    if(aux2[0] < aux4[0]){
        o1 = *r2;
        o2 = *r4;
    }else{
        o1 = *r4;
        o2 = *r2;
    }

    bmn(r3,&o1);

    bmn(&o1,&o2);

    *r2 = *r3;
    *r3 = o1;
    *r4 = o2;
}

//Funcion que implementa la logica del algoritmo de ordenamiento in-register, para obtener secuencias ordenadas
//Entrada:  -r1, Referencia registro SIMD con 4 valores flotantes
//          -r2, Referencia registro SIMD con 4 valores flotantes
//          -r3, Referencia registro SIMD con 4 valores flotantes
//          -r4, Referencia registro SIMD con 4 valores flotantes
void orderInRegister(__m128* r1, __m128* r2, __m128* r3, __m128* r4){

    //Sorting network
    sortingNetwork(r1, r2, r3, r4); 

    //Transpose
    transpose(r1, r2, r3, r4);

    //Bitonic Merge Network (BMN)
    bmn(r1, r2);
    bmn(r3, r4);
}

//Funcion que añade los 16 elementos (R1, R2, R3, R4) en una lista
//Entrada:  -list, Puntero de float en representación de una lista de largo 16
//          -r1, Referencia registro SIMD con 4 valores flotantes
//          -r2, Referencia registro SIMD con 4 valores flotantes          
//          -r3, Referencia registro SIMD con 4 valores flotantes
//          -r4, Referencia registro SIMD con 4 valores flotantes
void storeList(float * list, float * R1, float * R2, float * R3, float * R4){
    int i;
    for (i = 0; i < 4; i++)
    {   
       list[i] = R1[i];
    }
    for (i = 0; i < 4; i++)
    {
       list[i+4] = R2[i];
    }
    
    for (i = 0; i < 4; i++)
    {   

       list[i+8] = R3[i];
    }

    for (i = 0; i < 4; i++)
    {
       list[i+12] = R4[i];
    }

}

//Funcion que implementa la logica del algoritmo de ordenamiento Multiway Merge Sort(MWMS)
//Entrada:  -matriz, Puntero a puntero de float en representación de una lista con listas de largo 16
//          -listaFinal, Puntero a float en representacion de una lista de flotantes
//          -N, Entero que indica la cantidad de elementos de la listaFinal
void MWMS(float ** matriz, float * listaFinal, int N){
    int contador[N];
    memset(contador, 0, sizeof contador);

    int i;
    for (i = 0; i < N; i++)
    {    

        int posMin;
        int flag = 1;
        int j = 0;
        while(flag == 1){   
            if(contador[j] < 16){
                posMin=j;
                flag = 0;
            }
            j++;
        }

        float min = matriz[posMin][contador[posMin]];

        int m;
        for(m = posMin+1; m < N/16; m++){
            if(contador[m] < 16){
                if(min > matriz[m][contador[m]]){
                    min = matriz[m][contador[m]];
                    posMin = m;
                }
            }
        }

        listaFinal[i] = min;
        contador[posMin]++;
    }
    
}

//Funcion que implementa la logica del algoritmo de ordenamiento por Selección
//Entrada:  -array, Puntero de float en representación de una lista de floats
//          -N, Entero que indica la cantidad de elementos de la lista "array"
void SelectionSort(float *array, int n) {
  int x, y, min;
  float tmp;
  for(x = 0; x < n; x++) {
    min = x;
    for(y = x + 1; y < n; y++) {
      if(array[min] > array[y]) {
        min = y;
      }
    }
    tmp = array[min];
    array[min] = array[x];
    array[x] = tmp;
  }
}

//Funcion que 
//Entrada:  -array, Puntero de float en representación de una lista de floats
//          -
void divideYOrdenaras(float *array, int largo, int nivel) {
    printf("Numero de hebra = %d\n", omp_get_thread_num());
    

}
