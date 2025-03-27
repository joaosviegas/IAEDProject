/**
 * Neste ficheiro desenvolvi as estruturas Horas e Data, a fim de guardar e
 * verificar a validade dos dados introduzidos.
 * Juntamente desenvolvi funções que calculam a diferença entre estes tipos de dados,
 * que utilizei durante todo o projeto.
 * 
 * @file tempo.c
 * @author ist1109685
*/

#include "tempo.h"

/**
 * FUNÇÃO AUXILIAR - Recebe um vetor de inteiros e um
 * número inteiro, e verifica se esse número está no vetor.
 * 
 * @param vetor Vetor de inteiros.
 * @param n Número inteiro a procurar.
 * @param tamanho Número de elemetos do vetor. 
 * @return Devolve o resultado da procura.
 */
int aux_member(int vetor[], int n, int tamanho) {

    /* Certifica que o tamanho é do tipo int */
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i] == n)
            return TRUE;
    }
    return FALSE;
}

/* ----------------------- Inputs ----------------------- */

/**
 * Recebe uma data introduzida pelo utilizador e verifica se
 * é uma data válida.
 *
 * @param data Ponteiro para uma data do tipo Data.
 * @return O resultado da verificação.
 */
int leData(Data *data) {
    int meses_30[] = M30, meses_31[] = M31; // Expansão dos vetores M30 e M31
    scanf("%02d-%02d-%04d", &(data->dia), &(data->mes), &(data->ano));

    while (TRUE) {
        /* Verifica se o ano é válido*/
        if (data->ano <= 0) 
            break; /* Verifica se o número do mês */
        if (data->mes < meses_31[0] || data->mes > meses_31[6])
            break; /* Verifica fevereiro */
        else if (data->mes == 2 && data->dia > 28)
            break; /* Verifica os meses com 31 dias */
        else if (aux_member(meses_30, data->mes, SIZE_M30) && data->dia > 30)
            break; /* Verifica os meses com 31 dias */
        else if (aux_member(meses_31, data->mes, SIZE_M31) && data->dia > 31)
            break;
        else
            return TRUE;
    }
    printf("invalid date.\n");
    return FALSE;
}

/**
 * Recebe um ponteiro para uma data introduzida pelo utilizador
 * e verifica se é uma data válida.
 * 
 * @param h Ponteiro para uma hora do tipo Horas.
 * @return O resultado da verificação.
 */
int leHoras(Horas *h) {

    scanf("%d:%d", &(h->hora), &(h->minuto));
    /* Verifica se a hora é válida */
    if ( h->hora < 0 || h->hora > 23 || h->minuto < 0 || h->minuto > 59) {
        printf("invalid date.\n");
        return FALSE;
    }
    return TRUE;
}

/* ----------------------- Outputs ----------------------- */

/**
 * Escreve uma dada data.
 * 
 * @param data Pointeiro para uma data do tipo Data.
 */
void escreveData(Data *data) {
    printf("%02d-%02d-%04d ", data->dia, data->mes, data->ano);
}

/**
 * Escreve uma dada hora.
 * 
 * @param h Pointeiro para a hora h do tipo Horas.
 */
void escreveHoras(Horas *h) {
    printf("%02d:%02d", h->hora, h->minuto);
}

/* ----------------------- Funções ----------------------- */

/**
 * Calcula a diferença, em minutos, entre duas horas introduzidas.
 * 
 * @param h1 Hora de entrada.
 * @param h2 Hora de saída.
 * @return Int correspondente ao número de minutos que 
 * passou desde h1 até h2.
 */
int comparaHoras(Horas *h1, Horas *h2) {
    int min = 0;

    min = h2->minuto - h1->minuto; // Faz a diferença dos minutos
    min += (h2->hora - h1->hora) * HORA; // Faz a difernça das horas
    return min;
}

/**
 * Calcula o tempo, em minutos, decorrido entre duas data introduzidas.
 * 
 * @param d1 Primeira data (de entrada) do tipo Data.
 * @param d2 Segunda data (de saida) do tipo Data.
 * @return Int correspondente ao número de minutos
 * entre d1 e d2.
 */
int comparaData(Data *d1, Data *d2) {
    int dif = 0;
    int meses_30[] = M30, meses_31[] = M31;

    // Adiciona a diferença de anos, se houver
    dif += (d2->ano - d1->ano) * 365;

    if (d2->mes > d1->mes) { // Se o mês de saída for maior

        for (int i = 1; d2->mes - i >= d1->mes; i++) { // Decrementa até igualar
            /* Se for um mês com 30 dias, adiciona 30 */
            if (aux_member(meses_30, d2->mes - i, SIZE_M30))
                dif += 30;
            else if (aux_member(meses_31, d2->mes - i, SIZE_M31))
                dif += 31;
            else
                dif += 28; // Se chegar aqui, é feveriro e adiciona 28
        }       
    }
    else if (d2->mes < d1->mes) { // Se o mês de saída for menor

        for (int i = 1; d1->mes - i >= d2->mes; i++) {
            /* Retiramos os dias dos meses até igualar */
            if (aux_member(meses_30, d1->mes - i, SIZE_M30))
                dif -= 30;
            else if (aux_member(meses_31, d1->mes - i, SIZE_M31))
                dif -= 31;
            else
                dif -= 28;
        }
    }
    dif += d2->dia - d1->dia; // Calcula-se a diferença dos dias
    
    return dif * DIA; // Por último, converte dias para minutos
}

/**
 * Calcula a diferença em minutos entre duas datas e horas.
 *
 * @param d_ent Data de entrada.
 * @param h_ent Hora de entrada.
 * @param d_sai Data de saída.
 * @param h_sai Hora de saída.
 * @return A diferença em minutos entre as duas datas e horas.
 */
int calculaMinutos(Data *d_ent, Horas *h_ent, Data *d_sai, Horas *h_sai) {
    int dias, horas;
    
    dias = comparaData(d_ent, d_sai);
    horas = comparaHoras(h_ent, h_sai);
    return dias + horas;
}