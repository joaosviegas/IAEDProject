/**
 * Neste ficheiro implemetei uma estrutura Registo, que usa Linked Lists globais
 * para guardar todas a entradas e saidas do sistema, apesar de não ser muito 
 * eficiente, é intuitivo de apagar cada vez que um parque é removido.
 * 
 * @file registo.c
 * @author ist1109685
*/

#include "registo.h"

/* Ponteiro que assinala a criação da lista de registos. */
Registo *lista_reg = NULL;
/* Ponteiro para navegar pela lista de registos. */
Registo *head_reg = NULL;

/* ----------------------- Inputs ----------------------- */

/**
 * Lê os dados de entrada de um veículo para o seu registo.
 * 
 * @param registo Ponteiro para o registo criado.
 * @param v_sai Ponteiro para o veículo que está a entrar no parque.
 */
void leRegisto_entrada(Registo *registo, Veiculo *v_ent) {
    registo->parque = v_ent->parque;
    strcpy(registo->matricula, v_ent->matricula);
    /* Aloca espaço para uma cópia das datas e horas */
    Data *rdata_ent = (Data*) malloc(sizeof(Data));
    Horas *rhora_ent = (Horas*) malloc(sizeof(Horas));

    registo->data_ent = rdata_ent; // Associa aos vetores do registo
    registo->hora_ent = rhora_ent;
    /* Copia os dados para os vetores */
    memcpy(registo->data_ent, v_ent->data_ent, sizeof(Data));
    memcpy(registo->hora_ent, v_ent->hora_ent, sizeof(Horas));
}

/**
 * Lê os dados de saída de um veículo para o seu registo
 * correspondente, se existir.
 * 
 * @param registo Ponteiro para o registo a atualizar.
 * @param v_sai Ponteiro para o veículo que está a sair do parque.
 */
void leRegisto_saida(Registo *registo, Veiculo *v_sai) {
    int min = calculaMinutos(registo->data_ent, registo->hora_ent,
    v_sai->data_ent, v_sai->hora_ent);
    float fatura = calculaFaturacao(v_sai->parque, min);
    /* Mesma lógica do leRegistoEntrada */
    Data *rdata_sai = (Data*) malloc(sizeof(Data));
    Horas *rhora_sai = (Horas*) malloc(sizeof(Horas));

    registo->data_sai = rdata_sai;
    registo->hora_sai = rhora_sai;
    registo->valor_pago = fatura; // Adiciona a faturação paga

    memcpy(registo->data_sai, v_sai->data_ent, sizeof(Data));
    memcpy(registo->hora_sai, v_sai->hora_ent, sizeof(Horas));
}

/* ----------------------- Outputs ----------------------- */

/**
 * Escreve os dados de um dado registo.
 * 
 * @param registo Ponteiro para estrutura Registo.
 * 
 */
int escreveRegisto(Registo *registo) {
    printf("%s ",registo->parque->nome);
    escreveData(registo->data_ent);
    escreveHoras(registo->hora_ent);
    if (registo->valor_pago == INCOMPLETO) {
        printf("\n");
        return TRUE;
    }
    printf(" ");
    escreveData(registo->data_sai);
    escreveHoras(registo->hora_sai);
    printf("\n");
    return TRUE;
}

/**
 * Ordena e escreve todas as entradas e saídas do sistema de um dado
 * veículo, por ordem de nome do parque e de data de entrada.
 * 
 * @param v Ponteiro para o veículo cujas entradas e saídas queremos.
 */
void escreveEntradas(Veiculo *v) {
    Registo *lista[BUFSIZ] = {NULL};
    int i = 0, tamanho;
    if (lista_reg != NULL) {

        head_reg = lista_reg;
        while (head_reg != NULL) {
            /* Se for do veículo que pretendemos */
            if (!strcmp(head_reg->matricula, v->matricula)) {
                lista[i] = head_reg; // Adiciona-se a um vetor para ordenar
                i++;
            }
            head_reg = head_reg->next;
        }
        tamanho = i;
        if (tamanho == 0) { // Se não encontrar nenhum registo
            printf("%s: no entries found in any parking.\n", v->matricula);
        }
        else {
            bubbleRegisto(lista, tamanho); // Ordena os registos
            /* Escreve os registos */
            for (i = 0; i < tamanho; i++) {
                escreveRegisto(lista[i]);
            }
        }
    }
    else if (lista_reg == APAGADO) /* Se tiverem sido apagados os registos */
    printf("%s: no entries found in any parking.\n", v->matricula);
}

/**
 * Lê a matrícula de um veículo, e lista todas as suas
 * entradas e saídas, se for válida.
 * 
 * Comando 'v'.
 */
int listaEntradas() {
    Veiculo *v = (Veiculo*) calloc(1, sizeof(Veiculo));
    if (!leMatricula(v)) { // Se a matrícula for válida
        free(v);
        return FALSE;
    }
    else { // Se existir registo
        escreveEntradas(v);
    }
    free(v);
    return FALSE;
}

/* ----------------------- Funções ----------------------- */

/**
 * Cria um novo registo para um veículo a entrar num parque.
 * 
 * Utiliza Linked Lists.
 * 
 * @param v_ent Ponteiro para o veículo a entrar no parque.
 */
void criaRegisto_entrada(Veiculo *v_ent) {
    Registo *registo = (Registo*) calloc(1, sizeof(Registo));
    registo->next = NULL;
    registo->valor_pago = INCOMPLETO; // Maneira de saber se o registo está incompleto ou não

    if (lista_reg == NULL) { // Se ainda não existir registo
        leRegisto_entrada(registo, v_ent);
        lista_reg = registo;
    }
    else { // Senão, vai percorrendo-a para adicionar no fim
        head_reg = lista_reg;
    
        while (head_reg->next != NULL) {
            head_reg = head_reg->next;
        }
        leRegisto_entrada(registo, v_ent);
        head_reg->next = registo;
    }
}

/**
 * Verifica se o registo de um dado veículo existe no sistema.
 * 
 * @param v_sai Ponteiro para o veículo a sair do parque.
 * @return TRUE se o registo do veículo existir, FALSE em caso contrário.
 */
int existeRegisto(Veiculo *v_sai) {
    if (lista_reg == NULL) {
        return FALSE;
    }
    else {
        head_reg = lista_reg;
        /* Enquanto não ecncontar o registo de entrada correspondente */
        while (head_reg != NULL) {
            if (!strcmp(head_reg->matricula, v_sai->matricula) 
            && !strcmp(head_reg->parque->nome, v_sai->parque->nome)
            && (calculaMinutos(head_reg->data_ent, head_reg->hora_ent,
            v_sai->data_ent ,v_sai->hora_ent) >= 0)
            && head_reg->valor_pago == INCOMPLETO) {
                return TRUE;
            }
            head_reg = head_reg->next;
        }
        /* Se não encontrar o registo do veículo*/
        printf("%s: no entries found in any parking.\n", v_sai->matricula);
        return FALSE;
    }
}

/**
 * Atualiza um registo de um dado veículo a sair de um parque.
 * 
 * Utiliza Linked Lists.
 * 
 * @param v_sai Ponteiro para o veículo a sair do parque.
 */
void criaRegisto_saida(Veiculo *v_sai) {    
    if(existeRegisto(v_sai)) { // Se o registo existir
        head_reg = lista_reg;
        /* Enquanto não ecncontar o registo de entrada correspondente */
        while (head_reg != NULL) {
            if (!strcmp(head_reg->matricula, v_sai->matricula) 
        && !strcmp(head_reg->parque->nome, v_sai->parque->nome)
        && (calculaMinutos(head_reg->data_ent, head_reg->hora_ent,
        v_sai->data_ent ,v_sai->hora_ent) >= 0) && head_reg->valor_pago ==INCOMPLETO) {
            leRegisto_saida(head_reg, v_sai);
            }
            head_reg = head_reg->next;
        }
    }
}

/**
 * Obtem os dados de faturações desejados a partir dos
 * registos, e mostra-os ordenados.
 *
 * @param pnome Nome do parque cujos registos queremos.
 * @param data Data do dia cujos registos queremos.
 */
int obtemRegistos(char *pnome, Data *data) {
    Registo *lista[BUFSIZ] = {NULL};
    int i = 0, tamanho;
    if (lista_reg != NULL) {

        head_reg = lista_reg;
        while (head_reg != NULL) {
            if (head_reg->valor_pago != INCOMPLETO) {
                /* Se for do parque e da data que queremos */
                if ((comparaData(head_reg->data_sai, data) == 0)
                && !strcmp(pnome, head_reg->parque->nome)) {
                    lista[i] = head_reg; // Adiciona-se a um vetor para ordenar
                    i++;
                }
            }
            head_reg = head_reg->next;
        }
        tamanho = i;
        if (tamanho == 0) {
            return FALSE; // Se não houver registos, não faz nada
        }
        Registo **final = (Registo**) malloc(sizeof(Registo) * tamanho);
        memcpy(final, lista, sizeof(Registo)*tamanho);

        bubbleRegisto_faturacao(final, tamanho); // Ordenam-se os registos
        for (i = 0; i < tamanho; i++) {
                escreveFaturacao_2(final[i]);
        }
        free(final);
        return TRUE;
    }
    return FALSE;
}

/* ----------------------- Sort ----------------------- */

/**
 * Compara duas estruturas Registo tendo em conta o nome dos
 * seus parques e a data de entrada.
 * 
 * @param r1 Ponteiro para a primeira estrutura Registo.
 * @param r2 Ponteiro para a segunda estrutura Registo.
 * @return TRUE se r1 for maior do que r2, FALSE em caso contrário.
 */
int comparaRegistos(Registo *r1, Registo *r2) {
        int t1 = strlen(r1->parque->nome);
        int t2 = strlen(r2->parque->nome), i;
        int tmenor = (t1 < t2) ? t1 : t2;

        for (i = 0; i < tmenor; i++) {
            if (r2->parque->nome[i] == r1->parque->nome[i])
                continue;
            else
                return r2->parque->nome[i] > r1->parque->nome[i];
        }
        /* Se os nomes forem idênticos */
        if (comparaData(r1->data_ent, r2->data_ent) == 0) {

            if (comparaHoras(r1->hora_ent, r2->hora_ent) == 0) {
                return FALSE; // Se chegar aqui, são iguais, por isso é indiferente
            }
            else /* Se a diferença for positiva, o r2 é o maior */
                return (comparaHoras(r1->hora_ent, r2->hora_ent) > 0) ? TRUE : FALSE;
        }
        else
            return (comparaData(r1->data_ent, r2->data_ent) > 0) ? TRUE : FALSE;
}

/**
 * Compara duas estruturas Registo tendo em conta o nome dos
 * seus parques e a data de entrada.
 * 
 * @param r1 Ponteiro para a primeira estrutura Registo.
 * @param r2 Ponteiro para a segunda estrutura Registo.
 * @return TRUE se r1 for maior do que r2, FALSE em caso contrário.
 */
int comparaRegistos_faturacao(Registo *r1, Registo *r2) {

    if (comparaData(r1->data_sai, r2->data_sai) == 0) {

        if (comparaHoras(r1->hora_sai, r2->hora_sai) == 0) {
            return FALSE; // Se chegar aqui, são iguais, por isso é indiferente
        }
        else /* Se a diferença for positiva, o r2 é o maior */
            return (comparaHoras(r1->hora_sai, r2->hora_sai) > 0) ? TRUE : FALSE;
    }
    else
        return (comparaData(r1->data_sai, r2->data_sai) > 0) ? TRUE : FALSE;
}

/**
 * Troca a posição de dois dados registos num vetor.
 * 
 * @param r1 Ponteiro para pointeiro para o primeiro Registo.
 * @param r2 Ponteiro para pointeiro para o segundo Registo.
 */
void trocaRegistos(Registo **r1, Registo **r2) {
    Registo *auxiliar = *r2;
    *r2 = *r1;
    *r1 = auxiliar;
}

/**
 * Ordena um vetor de registos de acordo com o nome dos seus
 * parques, seguido das suas datas de entrada.
 * 
 * Utiliza o BubbleSort para tal efeito.
 * 
 * @param lista Ponteiro para ponteiro para o vetor de registos.
 * @param tamanho Tamanho do vetor a ordenar.
 */
void bubbleRegisto(Registo **lista, int tamanho) {
    int ordenado, i, j;

    for (i = 0; i < tamanho; i++) {
        ordenado = TRUE;
        for (j = 0; j + 1 < tamanho; j++) {
            if (!comparaRegistos(lista[j], lista[j + 1])) {
                trocaRegistos(&lista[j], &lista[j + 1]); // Trocam-se os registos
                ordenado = FALSE;
            }
        }
        if (ordenado)
            break;
    }
}

/**
 * Ordena um vetor de faturações de acordo com a data de saída.
 * 
 * Utiliza o BubbleSort para tal efeito.
 * 
 * @param lista Ponteiro para ponteiro para o vetor de faturações.
 * @param tamanho Tamanho do vetor a ordenar.
 */
void bubbleRegisto_faturacao(Registo **lista, int tamanho) {
    int ordenado, i, j;

    for (i = 0; i < tamanho; i++) {
        ordenado = TRUE;
        for (j = 0; j + 1 < tamanho; j++) {
            if (!comparaRegistos_faturacao(lista[j], lista[j + 1])) {
                trocaRegistos(&lista[j], &lista[j + 1]); // Trocam-se os registos
                ordenado = FALSE;
            }
        }
        if (ordenado)
            break;
    }
}

/* ----------------------- Destrutores ----------------------- */

/**
 * Destroi um registo e as suas dependências.
 *
 * @param registo Ponteiro para estrutura Registo.
 */
void destroiRegisto(Registo *registo) {
    free(registo->data_ent);
    free(registo->data_sai);
    free(registo->hora_ent);
    free(registo->hora_sai);
    registo->parque = NULL;
    free(registo);
}

/**
 * Destroi uma lista ligada de Registos.
 */
void destroiListaRegisto() {
    head_reg = lista_reg;
    if (head_reg != NULL) {

        while (lista_reg != NULL) { // Enquanto não chegar ao último
            head_reg = lista_reg->next; // Avança com a head
            destroiRegisto(lista_reg);
            lista_reg = head_reg; // Navega para o seguinte
        }
    }
}

/**
 * Escreve os dados de uma faturação, a partir de um dado registo, para o comando 'f'.
 *
 * @param registo Ponteiro para estrutura Registo.
 */
void escreveFaturacao_2(Registo *registo) {
    printf("%s ", registo->matricula);
    escreveHoras(registo->hora_sai);
    printf(" %.2f\n", registo->valor_pago); 
}


/**
 * Apaga um dado registo da Linked List.
 * 
 * @param head Ponteiro para ponteiro para a lista.
 */
void apagaReg(Registo **head) {
    Registo *anterior = *head;
    Registo *atual = *head;
    
    atual = atual->next; // Apontamos para o que queremos apagar
    anterior->next = atual->next; // Anterior aponta para o seguinte do que queremos apagar
    destroiRegisto(atual);
    atual = NULL;
}

/**
 * Apaga todos os Registos de um dado parque do sistema.
 * 
 * @param pnome Nome do parque a apagar.
 */
void apagaRegisto(char *pnome) {
    if (lista_reg != NULL) {
        while (TRUE) {
            int apagou = FALSE;
            head_reg = lista_reg;
            while(head_reg != NULL) { // Quando o seguinte for o que queremos apagar
                if (head_reg->next != NULL && !strcmp(pnome, head_reg->next->parque->nome)) {   
                    apagaReg(&head_reg);
                    apagou = TRUE;
                }
                head_reg = head_reg->next;
            }
            /* Se percorre tudo, verifica se era o primeiro elemento */
            if (lista_reg != NULL && lista_reg != APAGADO) {
                if (!apagou && !strcmp(pnome, lista_reg->parque->nome)) {
                    head_reg = lista_reg;
                    if (lista_reg->next != NULL)
                        lista_reg = lista_reg->next;
                    else {
                        lista_reg = APAGADO; // Se apagar o único elemento
                    }
                    destroiRegisto(head_reg);
                    apagou = TRUE;
                }
            }
            if (!apagou) // Quando fizer uma passagem e não alterar nada, acaba
                break;
        }
    }
}

/* ----------------------- NOVO COMANDO ----------------------- */

/**
 * Mostra a soma de todos os valores pagos por um dado 
 * veículo.
 * 
 * NOVO comando 'u'.
 */
void somaValor() {
    float soma = 0;
    Veiculo *v = (Veiculo*) calloc(1, sizeof(Veiculo));
    if (leMatricula(v)) { // Lê a matrícula;

        if (lista_reg != NULL) {
            head_reg = lista_reg;
            while (head_reg != NULL) {
                /* Se for do veículo que pretendemos */
                if (!strcmp(head_reg->matricula, v->matricula)
                && head_reg->valor_pago != INCOMPLETO) {
                    soma += head_reg->valor_pago; // Adiciona-se o valor pago
                }
                head_reg = head_reg->next;
            }
            printf("%.2f\n",soma); // Quando acaba, printa
            free(v);
        }
        else {
            free(v);
            printf("%.2f\n",soma);
        }
    }
}

