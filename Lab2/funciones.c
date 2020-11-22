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
#include <math.h>

#include "funciones.h"

struct Heap
{
    int lengthArray;
    float *array;
    int *idArray;               //arreglo de id a la lista que pertenece
    int *counterPosition;       //contador del elemento de la lista a la que pertenece
    int lastPosition;           //última posición con elemento
    
};


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


float ** SIMD_sort(float *array, int cantListas){
    float ** matrizListas = createMatriz(cantListas);

    __m128 r1,r2,r3,r4;

    for (int i = 0; i < cantListas; i++)
    {
        r1 = _mm_load_ps(array + (i*16));
        r2 = _mm_load_ps(array + (i*16) + 4);
        r3 = _mm_load_ps(array + (i*16) + 8);
        r4 = _mm_load_ps(array + (i*16) + 12);

        orderInRegister(&r1, &r2, &r3, &r4);
        mergeSIMD(&r1, &r2, &r3, &r4);

        float R1[4] __attribute__((aligned(16)));
        float R2[4] __attribute__((aligned(16)));
        float R3[4] __attribute__((aligned(16)));
        float R4[4] __attribute__((aligned(16)));

        _mm_store_ps(R1,r1);
        _mm_store_ps(R2,r2);
        _mm_store_ps(R3,r3);
        _mm_store_ps(R4,r4);
        storeList(matrizListas[i], R1, R2, R3, R4);
    }

    return matrizListas;
}

void insertHeap(float value, int indexList, struct Heap *heap)
{
    int sonIndex, fatherIndex;
    float auxValue;

    //Se inserta el elemento al final del heap
    heap->lastPosition = heap->lastPosition + 1;
    sonIndex = heap->lastPosition;
    heap->array[sonIndex] = value;

    //Se reordena el heap
    fatherIndex = (int)((round(sonIndex / 2.0)) - 1);

    while ((heap->array[sonIndex] < heap->array[fatherIndex]) && sonIndex > 0)
    {
        auxValue = heap->array[fatherIndex];
        heap->array[fatherIndex] = heap->array[sonIndex];
        heap->array[sonIndex] = auxValue;

        sonIndex = fatherIndex;
        fatherIndex = (int)((round(sonIndex / 2.0)) - 1);
    }

    heap->idArray[sonIndex] = indexList;
    heap->counterPosition[indexList]++;
}

void deleteHeap(struct Heap *heap){
    int fatherIndex, firstSonIndex, secondSonIndex;

    heap->array[0] = heap->array[heap->lastPosition];
    heap->array[heap->lastPosition] = -1;
    heap->lastPosition = heap->lastPosition - 1;

    fatherIndex = 0;
    firstSonIndex = 2 * fatherIndex + 1;
    secondSonIndex = 2 * fatherIndex + 2;
    
    while((firstSonIndex <= heap->lastPosition || secondSonIndex <= heap->lastPosition) && (heap->array[fatherIndex] > heap->array[firstSonIndex] || heap->array[fatherIndex] > heap->array[secondSonIndex])){
       if(firstSonIndex <= heap->lastPosition && secondSonIndex > heap->lastPosition){
            if(heap->array[fatherIndex] > heap->array[firstSonIndex]){
                swap(heap, &fatherIndex, firstSonIndex);
            }else{
                break;
            }
        }else if(secondSonIndex <= heap->lastPosition){
            if(heap->array[fatherIndex] > heap->array[firstSonIndex] && heap->array[fatherIndex] > heap->array[secondSonIndex]){
                if(heap->array[firstSonIndex] <= heap->array[secondSonIndex]){
                    swap(heap, &fatherIndex, firstSonIndex);
                }else{
                    swap(heap, &fatherIndex, secondSonIndex);
                }        
            }else if(heap->array[fatherIndex] > heap->array[firstSonIndex] && heap->array[fatherIndex] < heap->array[secondSonIndex]){
                swap(heap, &fatherIndex, firstSonIndex);
            }else if(heap->array[fatherIndex] < heap->array[firstSonIndex] && heap->array[fatherIndex] > heap->array[secondSonIndex]){
                swap(heap, &fatherIndex, secondSonIndex);
            }else{
                break;
            }
        }
        firstSonIndex = 2 * fatherIndex + 1;
        secondSonIndex = 2 * fatherIndex + 2;
    }
    
}

void assignFirstElement(struct Heap *heap, float *finalList, int *lengthFinalList)
{
    finalList[*lengthFinalList] = heap->array[0];
    *lengthFinalList++;
    deleteHeap(heap);

    /*for (int i = 0; i < lengthFinalList; i++)
    {
        if (finalList[i] == -1)
        {
            finalList[i] = heap->array[0];
            deleteHeap(heap);
            i = lengthFinalList;
        }
    }*/
}

void multiWaySort(float **listasOrdernadas, int cantidadListas)
{   
    struct Heap heap;
    heap.array = createList(cantidadListas);
    heap.idArray = (int*)malloc(sizeof(int) * cantidadListas);
    heap.lengthArray = cantidadListas;
    heap.lastPosition = -1;
    //int *contadores = (int*) malloc (sizeof(int*) * cantidadListas);
    heap.counterPosition = (int*) malloc (sizeof(int) * cantidadListas);
    memset(heap.counterPosition, 0, sizeof(int) * cantidadListas);

    float* finalList = createList(cantidadListas*16); 
    int lengthList = 0;

    printf("counter positions\n");
    for (int k = 0; k < cantidadListas; k++)
    {
        printf("%d - ", heap.counterPosition[k]);
    }
    printf("\n");

    //Inserción del primer elemento de cada lista
    for(int i = 0; i < cantidadListas; i++){
        insertHeap(listasOrdernadas[i][0], i, &heap);
    }

    //Remover el primer elemento del heap


    printf("HEAP\n");
    for (int k = 0; k < cantidadListas; k++)
    {
        printf("%f - ", heap.array[k]);
    }
    printf("\n");

    printf("INDEX\n");
    for (int k = 0; k < cantidadListas; k++)
    {
        printf("%d - ", heap.idArray[k]);
    }
    printf("\n");

    printf("counter positions2\n");
    for (int k = 0; k < cantidadListas; k++)
    {
        printf("%d - ", heap.counterPosition[k]);
    }
    printf("\n");

    

}

//Funcion que 
//Entrada:  -array, Puntero de float en representación de una lista de floats
//          -
void divideYOrdenaras(float *array, int largo, int nivel) {
    if(nivel == 0){
        
        if(largo < 16){
            printf("El nivel de recursividad ingresado genera listas inferiores a 16\nCancelando ejecución\n");
            exit(1);
        }

        int cantListas = largo/16;

        float ** listasOrdenadas = SIMD_sort(array, cantListas);

        

        for (int j = 0; j < cantListas; j++)
        {
            for (int i = 0; i < 16; i++)
            {   
                printf("%f - Lista %d - hebra %d \n", listasOrdenadas[j][i], j, omp_get_thread_num());
            }
            printf("\n");
        }

        multiWaySort(listasOrdenadas, cantListas);

        return;
    }
    else{
        
        int nuevoLargo = largo/2;

        #pragma omp task untied
        {   
            divideYOrdenaras(array, nuevoLargo, nivel-1);
        }

        #pragma omp task untied
        {
            divideYOrdenaras(array + nuevoLargo, nuevoLargo, nivel-1);
        }

        #pragma omp taskwait
        //merge()
    }

}
