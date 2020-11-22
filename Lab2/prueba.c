#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Heap
{
    int lengthArray;
    float *array;
    int *indexArray;
    int lastPosition; //última posición con elemento
    
};

float arregloBacan[2][16] = {{12, 21, 4, 13, 9, 8, 6, 7, 1, 14, 3, 0, 5, 11, 15, 10}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
float arreglo[16] = {12, 21, 4, 13, 9, 8, 6, 7, 1, 14, 3, 0, 5, 11, 15, 10};
float bacan[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

void insertHeap(float value, struct Heap *heap)
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
}

void swap(struct Heap *heap, int *fatherIndex, int sonIndex){
    float auxValue;
    auxValue = heap->array[*fatherIndex]; 
    heap->array[*fatherIndex] = heap->array[sonIndex];
    heap->array[sonIndex] = auxValue;
    *fatherIndex = sonIndex;
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

void assignFirstElement(struct Heap *heap, float *finalList, int lengthFinalList)
{
    for (int i = 0; i < lengthFinalList; i++)
    {
        if (finalList[i] == -1)
        {
            finalList[i] = heap->array[0];
            deleteHeap(heap);
            i = lengthFinalList;
        }
    }
}


void main()
{
    struct Heap heap;
    heap.array = (float*)malloc(sizeof(float) * 16); //TO DO 16 corregir cantidadListas
    heap.lengthArray = 16;
    heap.lastPosition = -1;

    float finalList[16]; //TO DO cantidad de listas*16
    for (int i = 0; i < 16; i++)
    {
        finalList[i] = -1;
    }

    //Hacer el for por cada elemento, llamando al insert en el heap
    for (int j = 0; j < 16; j++)
    {
        insertHeap(arreglo[j], &heap);
    }
    printf("LISTA\n");
    for (int k = 0; k < 16; k++)
    {
        printf("%f - ", heap.array[k]);
    }
    printf("\n");

    for(int i = 0; i < 16; i++){
        assignFirstElement(&heap, finalList, 16);
    }
    
    printf("NUEVA heap\n");
    for (int k = 0; k < 16; k++)
    {
        printf("%f - ", heap.array[k]);
    }
    printf("\n");

    printf("NUEVA lista final\n");
    for (int k = 0; k < 16; k++)
    {
        printf("%f - ", finalList[k]);
    }
    printf("\n");

}