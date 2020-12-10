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


//merge con prints
float * merge(float* arrayIzquierdo, float* arrayDerecho, int length){
    for (int i = 0; i < length; i++)
    {   
        printf("%f|%d\n", arrayIzquierdo[i], omp_get_thread_num());
    }
    
    printf("--------\n");

    for (int i = 0; i < length; i++)
    {
        printf("%f|%d\n", arrayDerecho[i], omp_get_thread_num());
    }

    float * mergeList = createList(length * 2);

    int izq = 0;
    int der = 0;

    while(izq + der < length *2){
        if(arrayIzquierdo[izq] < arrayDerecho[der] && izq < length){
            mergeList[izq+der] = arrayIzquierdo[izq];
            izq++;
        }else{
            mergeList[izq+der] = arrayDerecho[der];
            der++;
        }
    }
    return mergeList;
}


//Multiwaysort con traza
float* multiWaySort2(struct Element ** listasOrdernadas, int cantidadListas)
{   
    struct Heap heap;
    heap.array = createList(cantidadListas);
    heap.elements = (struct Element*) malloc (sizeof(struct Element) * cantidadListas);

    heap.lengthArray = cantidadListas;
    heap.lastPosition = -1;
    heap.counterPosition = (int*) malloc (sizeof(int) * cantidadListas);
    memset(heap.counterPosition, 0, sizeof(int) * cantidadListas);

    float* finalList = createList(cantidadListas*16); 
    int lengthList = 0;

    int indexList;


    //Inserción del primer elemento de cada lista
    for(int i = 0; i < cantidadListas; i++){
        insertHeap(listasOrdernadas[i][0], &heap);
    }

    printf("counter positions\n");
    for (int k = 0; k < cantidadListas; k++)
    {
        printf("%d - ", heap.counterPosition[k]);
    }
    printf("\n");


    printf("Heap inicial\n");
    for (int k = 0; k < cantidadListas; k++)
    {
        printf("%f - ", heap.elements[k].value);
    }
    printf("\n\n");



    for (int j = 0; j < cantidadListas*16; j++)
    {   

        //Remover el primer elemento del heap y se añade a la lista final
        indexList = assignFirstElement(&heap, finalList, &lengthList);
        
       /* printf("Heap resultante de remover elemento de la lista %d\n", indexList);
        for (int k = 0; k < cantidadListas; k++)
        {
            printf("%f|%d - ", heap.elements[k].value, heap.elements[k].index);
        }
        printf("\n");

        printf("counter positions2\n");
        for (int k = 0; k < cantidadListas; k++)
        {
            printf("%d - ", heap.counterPosition[k]);
        }
        printf("\n");
        printf("finalList\n");
        for (int k = 0; k < lengthList; k++)
        {
            printf("%f - ", finalList[k]);
        }
        printf("\n");*/

        if(heap.counterPosition[indexList] < 16){
            insertHeap(listasOrdernadas[indexList][heap.counterPosition[indexList]], &heap);
        }/*
        getchar();
        printf("Heap de insertar elemento de la lista %d\n", indexList);
        for (int k = 0; k < cantidadListas; k++)
        {
            printf("%f|%d - ", heap.elements[k].value, heap.elements[k].index);
        }
        printf("\n");
        getchar();*/
    }


    printf("counter positions2\n");
    for (int k = 0; k < cantidadListas; k++)
    {
        printf("%d - ", heap.counterPosition[k]);
    }
    printf("\n");

    printf("finalList\n");
    for (int k = 0; k < lengthList; k++)
    {
        printf("%f\n", finalList[k]);
    }
    printf("\n");
    
    return finalList;

}