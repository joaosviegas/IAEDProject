#ifndef _VEICULO_
#define _VEICULO_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tempo.h"
#include "parque.h"

/* Tamanho incial do HashMap. */
#define PRIMO 31
/* Estado para verificar se algo já foi apagado. */
#define APAGADO (void*) (NULL)

#define MAT 9 // Dimensão do nº de caracteres numa matrícula + '\0'

/* Estrutura para os veículos, incluíndo data e hora de entrada. */
typedef struct {
    char matricula[MAT];
    Data *data_ent;
    Horas *hora_ent;
    Parque *parque;
} Veiculo;

/* Estrutura para guardar os veículos. */
typedef struct {
    Veiculo **hashmap;
    int tamanho;
    int ocupados;
} Hashmap;

char *le_nome_parque();
int leMatricula(Veiculo *v);
char *le_nome_parque();
void escreveMatricula(Veiculo *v);

Hashmap *inicializaHashmap();
int hash(char matricula[MAT], int tamanho);

int adicionaVeiculo(Veiculo *v, Hashmap *hashmap);
Veiculo *procuraVeiculo(char matricula[MAT], Hashmap *hashmap);
int encontraVeiculo(char matricula[MAT], Hashmap *hashmap);
Veiculo *removeVeiculo(Veiculo *v, Hashmap *hashmap);

void destroiVeiculo(Veiculo *v);
void destroiHashmap(Hashmap *hashmap);
void duplicaTamanho(Hashmap *hashmap);

float calculaFaturacao(Parque *parque, int minutos);
int resetVeiculos(char *pnome, Hashmap *hashmap);

#endif