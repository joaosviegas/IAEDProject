/**
 * Neste ficheiro desenvolvi todas as implementações de estrutura Veículo, como 
 * também uma estrutura global Hashmap que guarda todos os veículos que entram 
 * e saiem do sistema.
 * 
 * @file veiculo.c
 * @author ist1109685
*/

#include "veiculo.h"

/* ----------------------- Inputs ----------------------- */

/**
 * Obtem input do utilizador e retorna o 
 * nome do parque inserido.
 * 
 * @return Ponteiro para o nome do parque lido.
 */
char *le_nome_parque() {
    char c, maximo[BUFSIZ]; // Tamanho máximo que uma linha pode tomar
    int i = 0;

    c = getchar(); // Passa o primeiro espaço á frente
    if ((c = getchar()) != '"') { // Se não encontar "
        maximo[i] = c;
        i++;
        /*Lê só a primeira palavra */
        while ((c = getchar()) != ' ' && c != '\n') {
            maximo[i] = c;
            i++;
        }
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
    strcpy(pnome, maximo); // Copia do buffer para o pointer
    return pnome;
}    

/**
 * Obtem input do utilizador e verifica se é uma matrícula
 * válida.
 * 
 * @param v Ponteiro para um Veiculo.
 * @return O resultado da verificação.
 */
int leMatricula(Veiculo *v) {
    int par_l = 0, par_d = 0, hifen = 0;

    scanf("%s", v->matricula);

    for (int i = 0; i < MAT && v->matricula[i] != '\0'; i++) {
        /* Verifica as letras */
        if (v->matricula[i] >= 'A' && v->matricula[i] <= 'Z') {
            par_l++;
            /* Verifica os números*/
        } else if (v->matricula[i] >= '0' && v->matricula[i] <= '9') {
            par_d++;
            /* Se não fore nem um nem outro, nem hífen, é inválida */
        } else if (v->matricula[i] != '-') {
            printf("%s: invalid licence plate.\n", v->matricula);
            return FALSE;
        }
        if (v->matricula[i] == '-') {
            hifen++;
        }
    } /* Verifica a contagem final */
    if (hifen != 2 || (par_d != 2 && par_l != 2 
    && par_d != 6 && par_l != 0)) { // Permite 3 pares de dígitos além do resto
        printf("%s: invalid licence plate.\n", v->matricula);
        return FALSE;
    }

    return TRUE;
}

/* ----------------------- Outputs ----------------------- */

/**
 * Escreve a matricula de um dado veiculo.
 * 
 * @param v Pointeiro para o Veiculo v.
 */
void escreveMatricula(Veiculo *v) {
    printf("%s ", v->matricula);
}

/* ----------------------- Construtores ----------------------- */

/**
 * Cria um Hashmap de Veiculos e inicializa todas as
 * posições a NULL.
 */
Hashmap *inicializaHashmap() {
    Hashmap *hashmap = (Hashmap*) malloc(sizeof(Hashmap));
    hashmap->hashmap = (Veiculo**) calloc(PRIMO, sizeof(Veiculo*));
    for (int i = 0; i < PRIMO; i++) {
        hashmap->hashmap[i] = NULL;
    }
    hashmap->tamanho = PRIMO; // Tamanho inicial
    hashmap->ocupados = 0;
    return hashmap;
}

/**
 * Duplica a capacidade de um hashmap quando a sua ocupação 
 * chega a metade.
 * 
 * @param hashmap Ponteiro para a estrutura que guarda os veículos.
 */
void duplicaTamanho(Hashmap *hashmap) {
    int i, j;

    /* Cria-se um novo vetor e inicializa-se */
    int novo_tamanho = hashmap->tamanho * 2; 
    Veiculo **novo = (Veiculo**) calloc(novo_tamanho, sizeof(Veiculo*));
    /* Copia-se os pointers do vetor antigo */
    for (i = 0; i < hashmap->tamanho; i++) {
        j = 0;
        if (hashmap->hashmap[i] != NULL && hashmap->hashmap[i] != APAGADO) {
            /* Reahash dos veiculos para o vetor novo */
            int indice = hash(hashmap->hashmap[i]->matricula, novo_tamanho);
            while (TRUE) {
                int tentativa = (indice + j * j) % novo_tamanho;
                if (novo[tentativa] == NULL && novo[tentativa] == APAGADO) {
                    novo[tentativa] = hashmap->hashmap[i];
                    break;
                }
                j++;
            } 
        }
    }
    free(hashmap->hashmap); // Liberta-se o antigo
    hashmap->hashmap = novo; // Associa-se ao novo
    hashmap->tamanho = novo_tamanho;
}

/**
 * Adiciona um veículo ao Hashmap, utilizando Quadratic Probbing
 * para resolver colisões.
 *
 * @param v Pointeira para estrutura Veiculo que se pretende adicionar.
 * @return O resultado da adição.
 */
int adicionaVeiculo(Veiculo *v, Hashmap *hashmap) {
    /* Calcula o hashing value */
    int indice = hash(v->matricula, hashmap->tamanho);

    for (int i = 0; i < hashmap->tamanho; i++) {
        /* Resolução de colisões */
        int tentativa = (indice + i * i) % hashmap->tamanho; // Quadratic Probbing
        if (hashmap->hashmap[tentativa] == NULL || hashmap->hashmap[tentativa] == APAGADO) {
            hashmap->hashmap[tentativa] = v;
            return TRUE;
        }
    }
    return FALSE;

}

/* ----------------------- Destrutores ----------------------- */

/**
 * Destrói um veículo e as suas dependências.
 *
 * @param v Ponteiro para estrutura Veiculo.
 */
void destroiVeiculo(Veiculo *v) {
    v->parque = NULL;
    free(v->data_ent); // Liberta a data e hora
    free(v->hora_ent);
    free(v); //Liberta o próprio veiculo
}

/**
 * Destroi um HashMap de Veiculos e liberta todos os veículos
 * nele guardados.
 * 
 * @param hashmap Ponteiro para a estrutura que guarda os veículos.
 */
void destroiHashmap(Hashmap *hashmap) {
    if (hashmap != NULL) {
        for (int i = 0; i < hashmap->tamanho; i++) {
            if (hashmap->hashmap[i] != NULL && hashmap->hashmap[i] != APAGADO) {
                destroiVeiculo(hashmap->hashmap[i]);
                hashmap->hashmap[i] = APAGADO; // Coloca a apagado
            }
        }
        free(hashmap->hashmap);
        free(hashmap);
    }
}

/* ----------------------- Funções ----------------------- */

/**
 * Calcula o hashing value de uma dada matrícula de um veículo.
 *
 * @param matricula A matrícula do veículo.
 * @return O hashing value calculado.
 */
int hash(char matricula[MAT], int tamanho) {
    int valor = 0;

    for (int i = 0; matricula[i] != '\0'; i++) {
        /* Conversão para unsigned int */
        valor = (PRIMO * valor + (unsigned char) matricula[i] ) % tamanho;
    }
    return valor % tamanho; // Devolve sempre dentro do tamanho do vetor
}

/**
 * Procura um veículo no Hashmap a partir de uma dada matrícula.
 *
 * @param matricula Matrícula do veículo queremos encontrar.
 * @param hashmap Ponteiro para estrutura que guarda os veículos.
 * 
 * @return Ponteiro para o veículo se ele existir, senão devolve NULL.
 */
Veiculo *procuraVeiculo(char matricula[MAT], Hashmap *hashmap) {
    int indice = hash(matricula, hashmap->tamanho);

    for (int i = 0; i < hashmap->tamanho; i++) {

        int tentativa = (indice + i * i) % hashmap->tamanho; // Quadratic Probbing
        if (hashmap->hashmap[tentativa] != NULL
        && hashmap->hashmap[tentativa] != APAGADO) {
            /* Quando a matrícula for igual */
            if (!strcmp(matricula, hashmap->hashmap[tentativa]->matricula))    
                return hashmap->hashmap[tentativa];
        }
    }
    return NULL;
}

/**
 * Procura um veículo no Hashmap a partir de uma dada matrícula.
 *
 * @param matricula Matrícula do veículo queremos encontrar.
 * @param hashmap Ponteiro para estrutura que guarda os veículos.
 * 
 * @return TRUE se o veículo existir, senão devolve FALSE.
 */
int encontraVeiculo(char matricula[MAT], Hashmap *hashmap) {
    int indice = hash(matricula, hashmap->tamanho);

    for (int i = 0; i < hashmap->tamanho; i++) {

        int tentativa = (indice + i * i) % hashmap->tamanho; // Quadratic Probbing
        if (hashmap->hashmap[tentativa] != NULL && hashmap->hashmap[tentativa] != APAGADO) {
            if (!strcmp(matricula, hashmap->hashmap[tentativa]->matricula)) {    
                return TRUE;
            }
        }
    }
    return FALSE;
}

/** 
 * Remove um veículo do Hashmap.
 *
 * @param v Pointeiro para estrutura Veiculo que se pretende remover.
 * @return Ponteiro para o veículo, se for removido, ou NULL em caso contrário.
 */
Veiculo *removeVeiculo(Veiculo *v, Hashmap *hashmap) {
    int indice = hash(v->matricula, hashmap->tamanho);

    for (int i = 0; i < hashmap->tamanho; i++) {
        int tentativa = (indice + i * i) % hashmap->tamanho;

        if (hashmap->hashmap[tentativa] != NULL && hashmap->hashmap[tentativa] != APAGADO) {
            if (!strcmp(v->matricula, hashmap->hashmap[tentativa]->matricula)) {
                destroiVeiculo(hashmap->hashmap[tentativa]);
                hashmap->hashmap[tentativa] = APAGADO;
                return hashmap->hashmap[tentativa]; 
            }
        }
    }
    return  NULL;
}

/** 
 * Remove todos os veículos que estavam num dado parque do Hashmap.
 *
 * @param pnome Pointeiro para nome do parque a remover.
 * @return TRUE, se forem removidos, ou FALSE em caso contrário.
 */
int resetVeiculos(char *pnome, Hashmap *hashmap) {
    int removidos = FALSE;
    for (int i = 0; i < hashmap->tamanho; i++) { // Percorre todo o hashmap

        if (hashmap->hashmap[i] != NULL && hashmap->hashmap[i] != APAGADO) {
            /* Quando o parque for o que se pretende, apaga */
            if (!strcmp(pnome, hashmap->hashmap[i]->parque->nome)) {
                destroiVeiculo(hashmap->hashmap[i]);
                hashmap->hashmap[i] = APAGADO; // Se encontrar APAGADO, para de procurar
                removidos = TRUE; // Devolve-se o ponteiro para libertar o conteúdo posteriormente
            }
        }
    }
    return  removidos;
}