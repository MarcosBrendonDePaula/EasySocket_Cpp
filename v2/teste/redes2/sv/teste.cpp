#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <random>


using namespace std;
#define TamA 1000000
#define TamM 2000001



bool Existe(long int Valores[], int tam, long int valor) {//chama a função existe e passa o vetor de valores o vetor que tem todos os valores
												//tam tamnho do vetor / verifica se o valor ta no vetor - pecore todo o vetor
	for (int i = 0; i < tam; i++) {
		if (Valores[i] == valor)
			return true;
	}
	return false;
}

void GeraAleatorios(long int numeros[], int quantNumeros, int Limite) {
	srand((unsigned int)time(NULL));
	long int v, i;
	for (i = 0; i < quantNumeros; i++) {
		v = (int)rand();

		while (Existe(numeros, i, v)) {
			v = (int)rand();
		}
		numeros[i] = v;
		printf("%d ", numeros[i]);
	}

}
int main()
{
	//FILE* geRar;
	//geRar = fopen("D:\\Down\\100000Numeros.txt", "w");

	//int numeros[TamA];
	long int* numeros;

	numeros = (long int*)malloc(TamM * sizeof(long int));

	GeraAleatorios(numeros, TamA, TamM);
	for (int i = 0; i < TamA; i++) {
		//fprintf(geRar, "%d ", numeros[i]);
		printf("%d ", numeros[i]);

	}

	return 0;

}