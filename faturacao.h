#ifndef _FATURA_
#define _FATURA_

#include "registo.h"

void escreveFaturacao_1(Faturacao *faturacao);
void escreveFaturacao(Veiculo *v_entrada, Veiculo *v_saida);
float calculaFaturacao(Parque *parque, int min);

void adicionaFaturacao_diaria(Faturacao **faturacao,
Veiculo *v_entrada, Veiculo *v_saida);

#endif