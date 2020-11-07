//
//  Autor: Joaquín Ignacio Jara Marín
//  Rut: 19.739.353-k
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

//Funcion que crea y asigna memoria una lista de float de tamaño N
//Entrada:  -N, entero que indica el largo de la lista a crear
//Salida:   -Puntero de float para la representacion de la lista
float * createList(int N){
    int i;
    float * lista = (float*)malloc(sizeof(float) * N);
    return lista;
}

//Funcion que crea y asigna memoria ...
//Entrada:  -N
//Salida:   -Puntero a puntero de float para la representacion de una matriz con memoria reservada
float ** createMatriz(int N){
    int i;
    float ** matrix = (float**)malloc(sizeof(float*) * N);

    for (i = 0; i < N; i++)
    {
        matrix[i] = (float*)malloc(sizeof(float) * 16);
    }

    return matrix;
}

//Funcion que lee una lista de numeros
//Entrada:  -cadena de char que almacena el nombre del archivo a leer
//          -N, entero que indica el largo de la lista
//Salida:   -Puntero a puntero de float para la representacion de una matriz con memoria reservada
float * readNumbers(char * name, int N){
    float a;
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


int s[16] = {12, 21, 4, 13, 9, 8, 6, 7, 1, 14, 3, 0, 5, 11, 15, 10};

void printRegister(__m128 r1){
    float R1[4] __attribute__((aligned(16)));
    _mm_store_ps(R1,r1);
    printf("%f %f %f %f\n",R1[0],R1[1],R1[2],R1[3]);
}

void printRegisters(__m128 r1, __m128 r2, __m128 r3, __m128 r4){
    printRegister(r1);
    printRegister(r2);
    printRegister(r3);
    printRegister(r4);
    printf("\n");
}

void sortingNetwork(__m128* r1, __m128* r2, __m128* r3, __m128* r4){
    __m128 r13min, r13max, r24min, r24max, r12min, r12max, r34min, r34max, r23min, r23max;
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

void mergeSIMD(__m128* r1, __m128* r2, __m128* r3, __m128* r4){


    __m128 min, max, o1, o2;

    bmn(r1,r3);

    min = _mm_min_ps(*r2, *r4);
    max = _mm_max_ps(*r2, *r4);

    o1 = _mm_shuffle_ps(min, max,_MM_SHUFFLE(1,0,1,0));
    o2 = _mm_shuffle_ps(min, max,_MM_SHUFFLE(3,2,3,2));

    bmn(r3,&o1);
    bmn(&o1,&o2);

    *r2 = *r3;
    *r3 = o1;
    *r4 = o2;
}


void orderInRegister(__m128* r1, __m128* r2, __m128* r3, __m128* r4){

    //Sorting network
    sortingNetwork(r1, r2, r3, r4);
    //printf("Matriz Cargada en registros:\n");
    //printRegisters(*r1,*r2,*r3,*r4);   

    //Transpose
    transpose(r1, r2, r3, r4);
    //printf("Matriz traspuesta:\n");
    //printRegisters(*r1,*r2,*r3,*r4);

    //Bitonic Merge Network (BMN)
    bmn(r1, r2);
    bmn(r3, r4);
    //printf("BMN\n");
    //printRegisters(*r1,*r2,*r3,*r4);
}


void storeList(float * list, int j, float * R1, float * R2, float * R3, float * R4){
    int i;

    for (i = 0; i < 4; i++)
    {   
       list[i+j] = R1[i];
    }
    for (i = 0; i < 4; i++)
    {
       list[i+j+4] = R2[i];
    }
    
    for (i = 0; i < 4; i++)
    {   

       list[i+j+8] = R3[i];
    }

    for (i = 0; i < 4; i++)
    {
       list[i+j+12] = R4[i];
    }
}

void MWMS(float ** matriz, float * listaFinal){

}

int main(int argc, char **argv)
{   
    char* i;
    char* o;
    int N = 0;
    int d = 0;
    int c1;

    while((c1 =  getopt(argc, argv,"i:o:N:d:")) != -1){
        switch(c1){
            case 'i':
                if(strcmp(optarg, "") == 0){
                    printf("Debe ingresar un nombre del archivo binario con la lista de entrada");
                    c1 = -1;
                    break;
                }
                else i = optarg;
                break;
            case 'o':
                if(strcmp(optarg, "") == 0){
                    printf("Debe ingresar un nombre para el archivo binario de salida");
                    c1 = -1;
                    break;
                }
                else o = optarg;
                break;
            case 'N':
                if(atof(optarg) < 1){
                    printf("El valor ingresado debe ser mayor que 0");
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
            case '?':
                if(optopt == 'i' || optopt == 'o' || optopt == 'N' || optopt == 'd')
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
    
    printf("Nombre archivo entrada: %s\n", i);
    printf("Nombre archivo salida: %s\n", o);
    printf("Largo de la lista: %i\n", N);
    printf("Valor debug: %i\n", d);

    int cantListas = N/16;
    float ** matrizListas = createMatriz(cantListas);

    
    // Lectura de archivos
    float * lista;        
    lista = readNumbers(i,N);

    //Etapa SIMD
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
        printRegisters(r1, r2, r3, r4);

        _mm_store_ps(R1,r1);
        _mm_store_ps(R2,r2);
        _mm_store_ps(R3,r3);
        _mm_store_ps(R4,r4);
        
        storeList(matrizListas[j/16], j, R1, R2, R3, R4);
        j = j + 16;
    }
        
    //Ordenamiento
    MWMS(matrizListas, lista);

    if (d == 1)
    {   
        int k;
        //Imprimir secuencia final
        for (k = 0; k < N; k++)
        {   
            //if (k%16 == 0)printf("-------------\n");
            //printf("%f\n", lista[k]);
        }
    }

    //Escribir archivo de salida

    

    return 0;
}