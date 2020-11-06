//
//	Autor: Joaquín Ignacio Jara Marín
//	Rut: 19.739.353-k
//	Curso: HPC
//	Profesor: Fernando Rannou
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <immintrin.h>
#include <time.h> 



//Funcion que crea y asigna memoria una lista de float de tamaño N
//Entrada:	-N, entero que indica el largo de la lista a crear
//Salida:	-Puntero de float para la representacion de la lista
float * createList(int N){
	int i;
	float * lista = (float*)malloc(sizeof(float) * N);
	return lista;
}

//Funcion que lee una lista de numeros
//Entrada:	-cadena de char que almacena el nombre de la imagen a leer
//			-N, entero que indica el largo de la lista
//Salida:	-Puntero a puntero de float para la representacion de una matriz con memoria reservada
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


int main(int argc, char **argv)
{	
	char* i;
    char* o;
	int N = 0;
	int d = 0;
	int c1;
	float * lista; 

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

	// Lectura de archivos
	lista = readNumbers(i,N);

	if (d == 1)
	{	
		//Imprimir secuencia final
	}

	//Escribir archivo de salida

	return 0;
}