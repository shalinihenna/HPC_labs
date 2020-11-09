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

float * createList(int N);
float ** createMatriz(int N);
float * readNumbers(char * name, int N);
void writeNumbers(char * name, float * lista, int N);
void printRegister(__m128 r1);
void printRegisters(__m128 r1, __m128 r2, __m128 r3, __m128 r4);
void sortingNetwork(__m128* r1, __m128* r2, __m128* r3, __m128* r4);
void transpose(__m128* r1, __m128* r2, __m128* r3, __m128* r4);
void bmn(__m128* r1, __m128* r2);
void mergeSIMD(__m128* r1, __m128* r2, __m128* r3, __m128* r4);
void orderInRegister(__m128* r1, __m128* r2, __m128* r3, __m128* r4);
void storeList(float * list, float * R1, float * R2, float * R3, float * R4);
void MWMS(float ** matriz, float * listaFinal, int N);
void SelectionSort(float *array, int n);
void freeMemory(float * lista, float ** matrizListas, int cantListas);

#endif