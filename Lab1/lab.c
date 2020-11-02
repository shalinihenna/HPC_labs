#include <stdio.h>
#include <mmintrin.h> /* MMX instrinsics __m64 integer type */
#include <xmmintrin.h> /* SSE __m128 float */
//#include <emmintrin.h>

int s[16] = {12, 21, 4, 13, 9, 8, 6, 7, 1, 14, 3, 0, 5, 11, 15, 10};

void printList(float registers[4]){
    printf("%f %f %f %f\n",registers[0],registers[1],registers[2],registers[3]);
}

void printRegisters(__m128 r1, __m128 r2, __m128 r3, __m128 r4){
    float R1[4] __attribute__((aligned(16)));
    float R2[4] __attribute__((aligned(16)));
    float R3[4] __attribute__((aligned(16)));
    float R4[4] __attribute__((aligned(16)));

    _mm_store_ps(R1,r1);
    _mm_store_ps(R2,r2);
    _mm_store_ps(R3,r3);
    _mm_store_ps(R4,r4);

    printList(R1);
    printList(R2);
    printList(R3);
    printList(R4);
    printf("\n");
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
    __m128 r13min, r13max, r24min, r24max, r12min, r12max, r34min, r34max, r23min, r23max;
    r13min = _mm_min_ps(r1,r3);
    r13max = _mm_max_ps(r1,r3);

    r24min = _mm_min_ps(r2,r4);
    r24max = _mm_max_ps(r2,r4);

    r1 = _mm_min_ps(r13min,r24min); //r12min
    r12max = _mm_max_ps(r13min,r24min);

    r34min = _mm_min_ps(r13max,r24max);
    r4 = _mm_max_ps(r13max,r24max); //r34max

    r2 = _mm_min_ps(r12max,r34min); //r23min
    r3 = _mm_max_ps(r12max,r34min); //r23max

    printf("Matriz Cargada en registros:\n");
    printRegisters(r1,r2,r3,r4);    

   //Transpose
    __m128 rAux, r1Mid, r2Mid, r3Mid, r4Mid;

    //-----------Paso 1-----------
    rAux = _mm_shuffle_ps(r1,r3,_MM_SHUFFLE(1,0,1,0));
    r1Mid = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    rAux = _mm_shuffle_ps(r1,r3,_MM_SHUFFLE(3,2,3,2));
    r2Mid = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    rAux = _mm_shuffle_ps(r2,r4,_MM_SHUFFLE(1,0,1,0));
    r3Mid = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    rAux = _mm_shuffle_ps(r2,r4,_MM_SHUFFLE(3,2,3,2));
    r4Mid = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    //-----------Paso 2-----------
    rAux = _mm_shuffle_ps(r1Mid,r3Mid,_MM_SHUFFLE(1,0,1,0));
    r1 = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));
    
    rAux = _mm_shuffle_ps(r1Mid,r3Mid,_MM_SHUFFLE(3,2,3,2));
    r2 = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));
    
    rAux = _mm_shuffle_ps(r2Mid,r4Mid,_MM_SHUFFLE(1,0,1,0));
    r3 = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    rAux = _mm_shuffle_ps(r2Mid,r4Mid,_MM_SHUFFLE(3,2,3,2));
    r4 = _mm_shuffle_ps(rAux,rAux,_MM_SHUFFLE(3,1,2,0));

    printf("Matriz traspuesta:\n");
    printRegisters(r1,r2,r3,r4);
}

void main(){
    orderInRegister();
    /*for(int i = 0; i < 16; i++){
        printf("List! %d\n",s[i]);
    }*/
    /*
    float R1[4] __attribute__((aligned(16))) = {0,1,2,3};
    float R2[4] __attribute__((aligned(16))) = {8,9,10,11};
    __m128 r1,r2,r3,r4;
    r1 = _mm_load_ps(R1);
    r2 = _mm_load_ps(R2);

    r3 = _mm_shuffle_ps(r1,r2,_MM_SHUFFLE(1,0,1,0));
    r4 = _mm_shuffle_ps(r3,r3,_MM_SHUFFLE(3,1,2,0));

    _mm_store_ps(R1,r4);
    printList(R1);
*/
}