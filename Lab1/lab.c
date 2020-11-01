#include <stdio.h>
#include <mmintrin.h> /* MMX instrinsics __m64 integer type */
#include <xmmintrin.h> /* SSE __m128 float */
//#include <emmintrin.h>

int s[16] = {12, 21, 4, 13, 9, 8, 6, 7, 1, 14, 3, 0, 5, 11, 15, 10};

void printList(float registers[4]){
    for(int i = 0; i < 4; i++){
        printf("Element %d: %f\n",i,registers[i]);
    }
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

    r12min = _mm_min_ps(r13min,r24min);
    r12max = _mm_max_ps(r13min,r24min);

    r34min = _mm_min_ps(r13max,r24max);
    r34max = _mm_max_ps(r13max,r24max);

    r23min = _mm_min_ps(r12max,r34min);
    r23max = _mm_max_ps(r12max,r34min);

    /*Ordenado: r12min
                r23min
                r23max
                r34max*/
    _mm_store_ps(R1,r12min);
    _mm_store_ps(R2,r23min);
    _mm_store_ps(R3,r23max);
    _mm_store_ps(R4,r34max);
    printf("HOLAA\n");
    printList(R1);
    printList(R2);
    printList(R3);
    printList(R4);
    

}

void main(){
    orderInRegister();
    /*for(int i = 0; i < 16; i++){
        printf("List! %d\n",s[i]);
    }*/
    
}