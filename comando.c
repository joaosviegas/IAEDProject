/**
 * Neste ficheiro desenvolvi implementações dos principais comandos, cujas 
 * definições noutros ficheiros estavam dependentes de vários outros, fiz 
 * este ficheiro a fim de evitar circular inclusions.
 * Utilizei variáveis globais para manter a hora do sistema sempre atual.
 * 
 * @file comando.c
 * @author ist1109685
*/

#include "comando.h"

/* Variáveis globais para controlar a data e hora de entrada. */
Data ultima_data = {0};
Horas ultima_hora = {0};

/* Flag para saber se o parque está vazio. */
int PRIMEIRO_VEICULO = TRUE;

/**
 * Obtem input do utilizador, verifica se é um Veiculo
 * válido e atualiza a hora global, se o for.
 * 
 * @param v Ponteiro para um Veiculo.
 * @return O resultado da verificação.
 */
int leVeiculo(Veiculo *v) {
    /* Se algum dos dados estiver incorreto, devolve FALSE */
    if (leMatricula(v) && leData(v->data_ent)
    && leHoras(v->hora_ent)) {
       if (PRIMEIRO_VEICULO) { // Se ainda não tiver entrado ninguém no sistema
        ultima_data = *(v->data_ent); // Atualiza a entrada
        ultima_hora = *(v->hora_ent);
        PRIMEIRO_VEICULO = FALSE;
        return TRUE;
        }
        /* Se a hora de entrada for igual ou maior á anterior */
        else if ((calculaMinutos(&ultima_data, &ultima_hora, 
        v->data_ent, v->hora_ent) >= 0)) {
            ultima_hora = *(v->hora_ent); // Atualiza a entrada
            ultima_data = *(v->data_ent);
            return TRUE;
        }
        else {
            printf("invalid date.\n");
            return FALSE;
        }

    }
    return FALSE;
}

/**
 * FUNÇÃO AUXILIAR - Destroi dados temporários associados á execução 
 * de funções.
 *
 * @param pnome Ponteiro para o nome de um parque.
 * @param v Ponteiro para estrutura veículo.
 */
void aux_libertaDados(char *pnome, Veiculo *v) {
    free(pnome);
    destroiVeiculo(v);
}

/**
 * FUNÇÃO AUXILIAR - Aloca memória para uma estrutura do tipo Data.
 * 
 * @return Ponteiro para a estrutura alocada.
 */
Data *aux_allocaData() {
    Data *pdata = (Data*) malloc(sizeof(Data));
    return pdata;
}

/**
 * FUNÇÃO AUXILIAR - Aloca memória para uma estrutura do tipo Horas.
 * 
 * @return Ponteiro para a estrutura alocada.
 */
Horas *aux_allocaHoras() {
    Horas *phoras = (Horas*) malloc(sizeof(Horas));
    return phoras;
}

/* ----------------------- Comandos ----------------------- */

/**
 * FUNÇÃO AUXILIAR - Aloca memória para uma estrutura do tipo Veiculo.
 * 
 * @return Ponteiro para a estrutura alocada.
 */
Veiculo *aux_allocaVeiculo() {
    Veiculo *v_saida = (Veiculo*) calloc(1, sizeof(Veiculo));
    v_saida->hora_ent = aux_allocaHoras();
    v_saida->data_ent = aux_allocaData();
    return v_saida;
}

/**
 * FUNÇÃO AUXILIAR - Atualiza o número de lugares livres num
 * parque e o número de lugares ocupados no Hashmap.
 * 
 * @param v Ponteiro para Veiculo a entrar no parque.
 * @param hashmap Ponteiro para estrutura que guarda os veículos.
 * 
 */
void aux_atualizaEntrada(Veiculo *v, Hashmap *hashmap) {
    v->parque->livres--; // Atualiza os lugares livres do parque
    hashmap->ocupados++;
}

/**
 * Adiciona um novo veículo ao sistema.
 * 
 * @param hashmap Ponteiro para estrutura que guarda os veículos.
 * @return Ponteiro para estrutura veículo se for bem sucedido, ou NULL.
 */
Veiculo *criaVeiculo(Hashmap *hashmap) {
    Veiculo *v = aux_allocaVeiculo();
    
    char *pnome = le_nome_parque(); // Obtem o nome do parque em que vai entrar

    if (!(existeLugarParque(pnome))) {
        aux_libertaDados(pnome, v);
        return NULL;
    }
    else if (!leVeiculo(v)) { // Se der erro ao criar o veículo, acaba
            aux_libertaDados(pnome, v);
            return NULL;
    } /* Se o veiculo ainda não estiver dentro do sistema */
    else if (!encontraVeiculo(v->matricula, hashmap)) {
        adicionaVeiculo(v, hashmap); // Adiciona o veiculo
        Parque *parque = procuraParque(pnome);
        v->parque = parque; // Aponta para o parque em que está
        aux_atualizaEntrada(v, hashmap);
        escreveParque(v->parque);
        criaRegisto_entrada(v); // Adiciona-se aos registos

        free(pnome);
        if (hashmap->ocupados >= hashmap->tamanho / 2)
            duplicaTamanho(hashmap);
        return v;
    }
    // Se já estiver dentro do sistema
    printf("%s: invalid vehicle entry.\n", v->matricula);
    aux_libertaDados(pnome, v);
    return NULL;
    
}

/**
 * FUNÇÃO AUXILIAR - Atualiza o número de lugares livres num
 * parque e o número de lugares ocupados no Hashmap.
 * 
 * @param v Ponteiro para Veiculo a sair do parque.
 * @param hashmap Ponteiro para estrutura que guarda os veículos.
 * 
 */
void aux_atualizaSaida(Veiculo *v, Hashmap *hashmap) {
    v->parque->livres++; // Atualiza os lugares livres do parque
    hashmap->ocupados--;
}

/**
 * Remove um carro do sistema e calcula o valor por ele a pagar.
 *  
 * @param hashmap Ponteiro para estrutura que guarda os veículos.
 * @return TRUE se for removido com sucesso, FALSE se ocorrer algum erro.
 */
int saidaVeiculo(Hashmap *hashmap) {
    Veiculo *v_saida = aux_allocaVeiculo();
    char *pnome = le_nome_parque(); // Obtem o nome do parque do qual vai sair

    if (!(existeParque(pnome)) || !leVeiculo(v_saida)) {
        aux_libertaDados(pnome, v_saida);
        return FALSE;
    }
    if (encontraVeiculo(v_saida->matricula, hashmap)) {
        Veiculo *v_entrada = procuraVeiculo(v_saida->matricula, hashmap);
        if (v_entrada != NULL && (calculaMinutos(v_entrada->data_ent, 
        v_entrada->hora_ent, v_saida->data_ent, v_saida->hora_ent) >= 0)) {
            ultima_hora = *(v_saida->hora_ent); // Atualiza a ultima data
            ultima_data = *(v_saida->data_ent);
            aux_atualizaSaida(v_entrada, hashmap);
            v_saida->parque = v_entrada->parque;
            adicionaFaturacao_diaria(&(v_saida->parque->fatura), v_entrada, v_saida);
            escreveFaturacao(v_entrada, v_saida);
            v_entrada->parque = NULL;
            removeVeiculo(v_entrada, hashmap); // Remove o veículo
            criaRegisto_saida(v_saida);
            aux_libertaDados(pnome, v_saida);
            return TRUE;
        }
        printf("invalid date.\n");
    } else /* Se o veículo não estiver dentro de nenhum parque */
        printf("%s: invalid vehicle exit.\n", v_saida->matricula);

    aux_libertaDados(pnome, v_saida);
    return FALSE;
}

/**
 * Lista a faturação diária de um parque.
 * 
 * Comando 'f' com 1 argumentos.
 *
 * @param pnome Ponteiro para o nome do parque em questão.
 */
void listaDias(char *pnome) {
    if (existeParque(pnome)) {
        Parque *parque = procuraParque(pnome);
        if (parque->fatura->total != VAZIO) {
            /* Percorre a lista e escreve as faturações */
            while (parque->fatura != NULL) {
                escreveFaturacao_1(parque->fatura);
                parque->fatura = parque->fatura->next;
            }
        }
    }
}

/**
 * Escreve todas as faturações diárias de um parque ou todas as faturações de
 * uma dada data.
 *  
 * Comando 'f'.
 * 
 * @return TRUE se for executado com sucesso, FALSE se ocorrer algum erro.
 */
int aux_listaFaturacao(char *pnome, int enter) {
    if (!(existeParque(pnome))) {
        free(pnome);
        return FALSE;
    } // Se encontrar um '\n'
    else if (!enter) {
        Data *data = (Data*) calloc(1, sizeof(Data));
        leData(data);
        /* Se for posterior á última data */
        if (comparaData(&ultima_data, data) > 0) {
            printf("invalid date.\n");
            free(data);
            free(pnome);
            return FALSE;
        }
        else { // Com 2 argumentos
            obtemRegistos(pnome, data);
            free(data);
            free(pnome);
            return TRUE;
        }
    }
    else { // Com 1 argumento
        listaDias(pnome);
        free(pnome);
        return FALSE;
    }
}

/**
 * Lê o input de um parque e/ou de uma data pelo utilizador e
 * mostra todas as faturações de um dado parque .
 *  
 * Comando 'f'.
 * 
 * @return TRUE se for executado com sucesso, FALSE se ocorrer algum erro.
 */
int listaFaturacao() {
    char c, maximo[BUFSIZ]; // Tamanho máximo
    int enter = FALSE, i = 0;

    c = getchar(); // Passa o primeiro espaço á frente
    if ((c = getchar()) != '"') { // Se não encontar "
        maximo[i] = c;
        i++;
        /* Lê só a primeira palavra */
        while ((c = getchar()) != ' ' && c != '\n') {
            maximo[i] = c;
            i++;
        }
        if (c == '\n') // Se não houver segundo argumento
            enter = TRUE;
    }
    else {
        /* Senão, lê até encontrar outra " */
        while ((c = getchar()) != '"') {
            maximo[i] = c; 
            i++;
        }
    }
    maximo[i] = '\0'; // Coloca o caracter final

    char *pnome = (char*) calloc(1, sizeof(char)*(i + 1));
    strcpy(pnome, maximo);

    return( aux_listaFaturacao(pnome, enter) );
    
}

/**
 * Remove um parque do sistema, juntamente com os seus 
 * registos, faturações e veículos.
 * 
 * Comando 'r'.
 * 
 * @param hashmap Hashmap que contém os veículos.
 * @return TRUE se o parque foi removido, FALSE em caso contrário.
 */
int removeParque(Hashmap *hashmap) {

    char *pnome = le_nome_parque(); // Lê o nome do parque
    if (existeParque(pnome)) { // Apaga o parque com esse nome, se existir
        resetVeiculos(pnome, hashmap);
        apagaRegisto(pnome);
        apagaParque(pnome);
        listaParques_apagado(); // Lista os parques que sobram
        free(pnome);
        return TRUE;
    }
    free(pnome);
    return FALSE;
}
