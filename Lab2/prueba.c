#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float arregloBacan[2][16] = {{12, 21, 4, 13, 9, 8, 6, 7, 1, 14, 3, 0, 5, 11, 15, 10}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
float arreglo[16] = {12, 21, 4, 13, 9, 8, 6, 7, 1, 14, 3, 0, 5, 11, 15, 10};
float bacan[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

void insertHeap(float value, float *heap, int lengthList)
{
    int sonIndex, fatherIndex;

    //Se inserta el elemento al final del heap
    for (int i = 0; i < lengthList; i++)
    {
        if (heap[i] < 0)
        {
            heap[i] = value;
            sonIndex = i;
            i = lengthList;
        }
    }

    //Se reordena el heap
    fatherIndex = (int)((round(sonIndex / 2.0)) - 1);
    printf("\n");
    if (sonIndex > 0)
    {
        while (heap[sonIndex] < heap[fatherIndex])
        {
            printf("son Index: %d\n", sonIndex);
            printf("father Index: %d\n\n", fatherIndex);
            float aux = heap[fatherIndex];
            heap[fatherIndex] = heap[sonIndex];
            heap[sonIndex] = aux;

            sonIndex = fatherIndex;
            fatherIndex = (int)((round(sonIndex / 2.0)) - 1);
            if (sonIndex == 0)
                break;
        }
    }
}

void multiWaySort(int length)
{
    int lengthList = length / 16;
}

void main()
{
    float heap[16];
    for (int i = 0; i < 16; i++)
    {
        heap[i] = -1;
    }
    //Hacer el for por cada elemento, llamando al insert en el heap
    for (int j = 0; j < 16; j++)
    {
        insertHeap(arreglo[j], heap, 16);
    }

    for (int k = 0; k < 16; k++)
    {
        printf("%f - ", heap[k]);
    }
    printf("\n");
}