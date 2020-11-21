#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Heap
{
    float array[16];
    int lastPosition; //última posición con elemento
    int lengthArray;
};

//Verificar si la lista inicial pueden inicializarse con valores menores que 0.
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
    printf("INICIO\n");
    for (int k = 0; k < 16; k++)
    {
        printf("%f - ", heap->array[k]);
    }
    printf("\n\n");

    //Se reordena el heap
    fatherIndex = (int)((round(sonIndex / 2.0)) - 1);

    while ((heap->array[sonIndex] < heap->array[fatherIndex]) && sonIndex > 0)
    {
        auxValue = heap->array[fatherIndex];
        heap->array[fatherIndex] = heap->array[sonIndex];
        heap->array[sonIndex] = auxValue;
        printf("WHILE\n");
        for (int k = 0; k < 16; k++)
        {
            printf("%f - ", heap->array[k]);
        }
        printf("\n\n");

        sonIndex = fatherIndex;
        fatherIndex = (int)((round(sonIndex / 2.0)) - 1);
    }
}

void deleteHeap(float *heap, int lengthHeap)
{
    int fatherIndex, firstSonIndex, secondSonIndex;
    float auxValue;

    //Esta malo e imprimir la lista final
    heap[0] = heap[lengthHeap - 1];
    heap[lengthHeap - 1] = -1;

    fatherIndex = 0;
    firstSonIndex = 2 * fatherIndex + 1;
    secondSonIndex = 2 * fatherIndex + 2;

    while ((fatherIndex < lengthHeap && (firstSonIndex < lengthHeap || secondSonIndex < lengthHeap)) && ((heap[fatherIndex] > heap[firstSonIndex]) || (heap[fatherIndex] > heap[secondSonIndex])))
    {
        printf("LISTA\n");
        for (int k = 0; k < 16; k++)
        {
            printf("%f - ", heap[k]);
        }
        printf("\n");

        //Para ver el mínimo
        if ((heap[secondSonIndex] == -1 && heap[firstSonIndex] >= 0) || (heap[firstSonIndex] < heap[secondSonIndex]))
        {
            auxValue = heap[fatherIndex];
            heap[fatherIndex] = heap[firstSonIndex];
            heap[firstSonIndex] = auxValue;

            fatherIndex = firstSonIndex;
        }
        else if ((heap[firstSonIndex] == -1 && heap[secondSonIndex] >= 0) || (heap[firstSonIndex] > heap[secondSonIndex]))
        {
            auxValue = heap[fatherIndex];
            heap[fatherIndex] = heap[secondSonIndex];
            heap[secondSonIndex] = auxValue;

            fatherIndex = secondSonIndex;
        }
        firstSonIndex = 2 * fatherIndex + 1;
        secondSonIndex = 2 * fatherIndex + 2;
    }
}

void assignFirstElement(float *heap, float *finalList, int lengthFinalList, int lengthHeap)
{
    for (int i = 0; i < lengthFinalList; i++)
    {
        if (finalList[i] == -1)
        {
            finalList[i] = heap[0];
            deleteHeap(heap, lengthHeap);
            i = lengthFinalList;
        }
    }
}

void multiWaySort(int length)
{
    int lengthList = length / 16;
}

void main()
{
    struct Heap heap;
    heap.lengthArray = 16;
    heap.lastPosition = -1;

    float finalList[16];
    for (int i = 0; i < 16; i++)
    {
        finalList[i] = -1;
    }

    //Hacer el for por cada elemento, llamando al insert en el heap
    for (int j = 0; j < 16; j++)
    {
        insertHeap(bacan[j], &heap);
    }

    for (int k = 0; k < 16; k++)
    {
        printf("%f - ", heap.array[k]);
    }
    printf("\n");

    /*assignFirstElement(heap, finalList, 16, 16);
    printf("NUEVA LISTA\n");
    for (int k = 0; k < 16; k++)
    {
        printf("%f - ", heap[k]);
    }
    printf("\n");*/
}