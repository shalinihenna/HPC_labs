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

#ifndef FUNCIONES_H
#define FUNCIONES_H

struct Element 
{
    float value;
    int index;
};


struct Heap
{
    int lengthArray;
    float * array;
    struct Element * elements;
    int *counterPosition;       //contador del elemento de la lista a la que pertenece
    int lastPosition;           //última posición con elemento
    
};

float * createList(int N);
struct Element ** createStructList(int N);
float * readNumbers(char * name, int N);
void writeNumbers(char * name, float * lista, int N);
void printRegister(__m128 r1);
void printRegisters(__m128 r1, __m128 r2, __m128 r3, __m128 r4);
void sortingNetwork(__m128* r1, __m128* r2, __m128* r3, __m128* r4);
void transpose(__m128* r1, __m128* r2, __m128* r3, __m128* r4);
void bmn(__m128* r1, __m128* r2);
void mergeSIMD(__m128* r1, __m128* r2, __m128* r3, __m128* r4);
void orderInRegister(__m128* r1, __m128* r2, __m128* r3, __m128* r4);
void storeList(struct Element * list, int indexList, float * R1, float * R2, float * R3, float * R4);
struct Element ** SIMD_sort(float *array, int cantListas);
void swap(struct Heap *heap, int *fatherIndex, int sonIndex);
void insertHeap(struct Element object, struct Heap *heap);
void deleteHeap(struct Heap *heap);
int assignFirstElement(struct Heap *heap, float *finalList, int *lengthFinalList);
float* multiWaySort(struct Element **listasOrdernadas, int cantidadListas);
void divideYOrdenaras(float *array, int largo, int nivel);
void merge(float* array, int length);
#endif