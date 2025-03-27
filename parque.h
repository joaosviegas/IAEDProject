#ifndef _PARQUE_
#define _PARQUE_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tempo.h"

/* Valor para saber se uma faturação está vazia. */
#define VAZIO (float) 999999
#define MAX_P 20 // Número máximo de parques no sistema

/* Estrutura para guardar a faturação diária de um parque. */
typedef struct stru_faturacao {
    Data *data;
    float total;
    struct stru_faturacao *next;
} Faturacao;

/* Estrutura para os parques de estacionamento. */
typedef struct stru_parque {
    int capacidade,
    livres;
    float valor_15, valor_apos_15, max_diario;
    struct stru_parque *next;
    char *nome;
    Faturacao *fatura;
} Parque;

int leParque(Parque *parque);
int criaParque();

Parque *procuraParque(char nome[]);
int existeLugarParque(char nome[]);
int existeParque(char nome[]);

void listaParques(); 
void listaParques_apagado();
void escreveParque(Parque *parque);
void destroiLista();

void bubbleParques(Parque **lista, int tamanho);

int apagaParque(char *pnome);

Faturacao *inicialiazaFaturacao();
void destroiListaFaturacao(Faturacao **faturacao);

#endif