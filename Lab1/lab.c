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

void orderInRegister(){

    //Loading Registers
    __m128 r1,r2,r3,r4;
    float R1[4] __attribute__((aligned(16))) = {s[0],s[1],s[2],s[3]};
    float R2[4] __attribute__((aligned(16))) = {s[4],s[5],s[6],s[7]};
    float R3[4] __attribute__((aligned(16))) = {s[8],s[9],s[10],s[11]};
    float R4[4] __attribute__((aligned(16))) = {s[12],s[13],s[14],s[15]};

    r1 = _mm_load_ps(R1);
    r2 = _mm_load_ps(R2);
    r3 = _mm_load_ps(R3);
    r4 = _mm_load_ps(R4);

    //Sorting network
    sortingNetwork(&r1,&r2,&r3,&r4);
    printf("Matriz Cargada en registros:\n");
    printRegisters(r1,r2,r3,r4);   

    //Transpose
    transpose(&r1,&r2,&r3,&r4);
    printf("Matriz traspuesta:\n");
    printRegisters(r1,r2,r3,r4);

    //Bitonic Merge Network (BMN)
    bmn(&r1,&r2);
    bmn(&r3,&r4);
    printf("BMN\n");
    printRegisters(r1,r2,r3,r4);
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

    float * lista; 
    int cantListas = N/16;
    
    // Lectura de archivos
    lista = readNumbers(i,N);

    if (d == 1)
    {   
        //Imprimir secuencia final
    }

    //Escribir archivo de salida

    orderInRegister();

    return 0;
}