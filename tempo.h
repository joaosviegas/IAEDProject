#ifndef _TEMPO_
#define _TEMPO_

#include <stdlib.h>
#include <stdio.h>

#define TRUE 1 // Booleanos para aumentar a legibilidade
#define FALSE 0

/* Unidades de tempo para faturação em minutos. */
#define QUARTO 15 
#define HORA 60
#define DIA 60*24

#define M30 {4, 6, 9, 11} // Nº dos meses com 30 e 31 dias, respetivamente
#define M31 {1, 3, 5, 7, 8, 10, 12}
#define SIZE_M30 4 // Tamanho dos vetores acima
#define SIZE_M31 7

/* Estrutura para as datas. */
typedef struct {
    int dia, mes, ano;
} Data;

/* Estrutura para as horas. */
typedef struct {
    int hora, minuto;
} Horas;

int leData(Data *data);
int leHoras(Horas *h);

void escreveData(Data *data);
void escreveHoras(Horas *h);

int comparaHoras(Horas *h1, Horas *h2);
int comparaData(Data *d1, Data *d2);
int calculaMinutos(Data *d_ent, Horas *h_ent, Data *d_sai, Horas *h_sai);

#endif