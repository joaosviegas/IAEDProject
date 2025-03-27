/**
 * Neste ficheiro desenvolvi uma estrutura Faturacao para guardar as faturações 
 * diárias de cada parque por dias, devo dizer que esta funcionalidade foi a 
 * última a ser desenvolvida, pelo que não consegui despistar por completo os 
 * erros de memória.
 * Utilizei Linked List dentro de cada estrutura Parque mantendo a lista por 
 * ordem cronológica.
 * 
 * @file faturacao.c
 * @author ist1109685
*/

#include "faturacao.h"

/**
 * Escreve os dados de um dado veículo e valor a pagar à 
 * saída de um parque.
 * 
 * Comando 's'.
 * 
 * @param v_entrada Pointeiro para o veículo à entrada do parque.
 * @param v_saida Pointeiro para o veículo à saída do parque.
 */
void escreveFaturacao(Veiculo *v_entrada, Veiculo *v_saida) {
    int min = calculaMinutos(v_entrada->data_ent, v_entrada->hora_ent,
    v_saida->data_ent, v_saida->hora_ent);
    float fatura = calculaFaturacao(v_entrada->parque, min);

    escreveMatricula(v_entrada);
    escreveData(v_entrada->data_ent);
    escreveHoras(v_entrada->hora_ent);
    printf(" ");
    escreveData(v_saida->data_ent);
    escreveHoras(v_saida->hora_ent);
    printf(" %.2f\n", fatura);
}

/**
 * Calcula o valor a pagar por um veículo à saída de um dado parque.
 *
 * @param parque Ponteiro para estrutura parque.
 * @param min Quantidade, em minutos, de tempo que o carro ficou lá estacionado.
 * 
 * @return Float que representa o valor a pagar, em euros.
 */
float calculaFaturacao(Parque *parque, int min) {
    float faturacao = 0, acumulador = 0;
    int primeira_hora = 0;

    /* Adicionam-se os períodos de 24 horas */
    while (min >= DIA) {
        faturacao += parque->max_diario;
        min -= DIA;
    }
    /* Adiciona-se, período a período, a um acumulador */
    while (min > 0) {
        if (primeira_hora != 4) { // Flag para a primeira hora
            acumulador += parque->valor_15;
            min -= QUARTO;
            primeira_hora ++;
        }
        else {
            acumulador += parque->valor_apos_15;
            min -= QUARTO; // Se exceder, adiciona o máximo
            if (acumulador >= parque->max_diario) {
                acumulador = parque->max_diario;
                break;
            }
        }
    }
    faturacao += acumulador; // Adiciona-se o acumulador e devolve a faturação
    return faturacao;
}

/**
 * Alloca memória para uma estrtutura do tipo Faturação e os seus parâmetros.
 * 
 * @return Ponteiro para a estrutura allocada.
 */
Faturacao *aux_allocaFaturacao() {
    Faturacao *nova_faturacao = (Faturacao*) calloc(1, sizeof(Faturacao));
    nova_faturacao->next = NULL;
    Data *pdata = (Data*) calloc(1, sizeof(Data));
    nova_faturacao->data = pdata;
    return nova_faturacao;
}

/**
 * Adiciona os dados de faturação pagos por um veícula à saída de um dado parque, ao total do valor faturado pelo parque.
 *
 * @param lista Ponteiro para ponteiro para o primeiro elemento da Linked List.
 * @param v_entrada Ponteiro para veiculo à entrada do parque.
 * @param v_saida Ponteiro para veiculo à saída do parque.
 */
void adicionaFaturacao_diaria(Faturacao **lista, Veiculo *v_entrada, Veiculo *v_saida) {
    Faturacao *faturacao;
    faturacao = *lista;
    int min = calculaMinutos(v_entrada->data_ent, v_entrada->hora_ent,
    v_saida->data_ent, v_saida->hora_ent);
    int adicionou = FALSE;
    float fatura = calculaFaturacao(v_entrada->parque, min);
    /* Se ainda não existirem faturações */
    if (faturacao->total == VAZIO) {
        memcpy(faturacao->data, v_saida->data_ent, sizeof(Data));
        faturacao->total = fatura;
        adicionou = TRUE;
    }
    else { /* Procura-se a faturação com a data correspondente */
        while (faturacao->next != NULL) {
            if (comparaData(faturacao->data, v_saida->data_ent) == 0)
                break;
            faturacao = faturacao->next;
        } /* Se ainda não tiver adicionado */
        if (comparaData(faturacao->data, v_saida->data_ent) == 0) {
            faturacao->total = faturacao->total + fatura;
            adicionou = TRUE;
        } /* Senão encontrar, cria-se uma nova */
        else if (!adicionou) {
            Faturacao *nova_faturacao = aux_allocaFaturacao();
            memcpy(nova_faturacao->data, v_saida->data_ent, sizeof(Data));
            nova_faturacao->total = fatura;
            faturacao->next = nova_faturacao;
        }
    }
}

/**
 * Escreve os dados de uma faturação, para o comando 'f'.
 * 
 * COmando 'f' com 1 argumento.
 *
 * @param faturacao Ponteiro para faturação.
 */
void escreveFaturacao_1(Faturacao *faturacao) {
    escreveData(faturacao->data);
    printf("%.2f\n", faturacao->total); 
}