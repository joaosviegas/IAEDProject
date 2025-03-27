#ifndef _REGISTO_
#define _REGISTO_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tempo.h"
#include "parque.h"
#include "veiculo.h"

/* Valor para saber se um registo está completo ou não. */
#define INCOMPLETO 99999

typedef struct stru_registo {
    Parque *parque;
    char matricula[MAT];
    Data *data_ent;
    Horas *hora_ent;
    Data *data_sai;
    Horas *hora_sai;
    float valor_pago;
    struct stru_registo *next;
} Registo;

int listaEntradas();

void criaRegisto_entrada(Veiculo *v_ent);
void criaRegisto_saida(Veiculo *v_sai);
int existeRegisto(Veiculo *v_sai);
int obtemRegistos(char *pnome, Data *data);
void escreveFaturacao_2(Registo *registo);

int comparaRegistos(Registo *r1, Registo *r2);
void bubbleRegisto(Registo **lista, int tamanho);
void bubbleRegisto_faturacao(Registo **lista, int tamanho);

void destroiListaRegisto();
void apagaRegisto(char *pnome);



void somaValor();

#endif