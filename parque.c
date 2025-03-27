/**
 * Neste ficheiro implemetei uma estrutura Parque, guardada numa Linked List, 
 * com uma dimensão máxima de 20 parques, que nunca poderia ser excedido.
 * Também implementei neste ficheiro alguns aspetos da faturação diária do parque
 * que estavam dependentes deste ficheiro, novamente para evitar circular inclusions.
 * 
 * @file parque.c
 * @author ist1109685
*/

#include "parque.h"
#include "veiculo.h"

/* Ponteiro que assinala a criação da lista de parques. */
Parque *lista = NULL;
/* Ponteiro para navegar pela lista de parques. */
Parque *head = NULL;

/* Contador para o número de parques no sistema. */
int CONTADOR = 0;

/* ----------------------- Inputs ----------------------- */

/**
 * Lê o input para os dados de um parque.
 * 
 * @param parque Ponteiro para estrutura Parque.
 */
int leParque(Parque *parque) {
    char c, maximo[BUFSIZ]; // Tamanho máximo que uma linha pode tomar
    int i = 0;
    while ((c = getchar()) != EOF && c != '\n') {
        
        maximo[i] = c; // Adiciona cada caracter ao vetor
        i++;
    }
    maximo[i] = '\0'; // Coloca o caracter final
    
    char *pnome = (char*) calloc(1, sizeof(char)*(i + 1));
    parque->nome = pnome;
    /* Se não encontrar nada com aspas, lê normalmente */
    if (!sscanf(maximo, "\"%[^\"]\" %d %f %f %f", pnome, 
               &parque->capacidade, &parque->valor_15, 
               &parque->valor_apos_15, &parque->max_diario)) {

        sscanf(maximo, "%s %d %f %f %f", pnome, 
        &parque->capacidade, &parque->valor_15, 
        &parque->valor_apos_15, &parque->max_diario);
    }
    i = 0;
    while (pnome[i] != '\0') { // Percorre o nome lido
        /* Se tiver algum dígito, dá erro */
        if (pnome[i] >= '1' && pnome[i] <= '9') {
            printf("invalid parking name.\n");
            return FALSE;
        }
        i++;
    }
    return TRUE;
}

/* ----------------------- Destrutores ----------------------- */

/**
 * Destroi um parque e as suas dependências.
 *
 * @param parque Ponteiro para estrutura Parque.
 */
void destroiParque(Parque *parque) {
    destroiListaFaturacao(&(parque->fatura));
    free(parque->nome);
    free(parque);
}

/**
 * Destroi uma lista de Parques.
 * 
 * @param head Ponteiro para a head da Linked List.
 */
void destroiLista() {
    head = lista;
    if (head != NULL) {

        while (lista != NULL) { // Enquanto não chegar ao último
            head = lista->next; // Avança com a head
            destroiParque(lista);
            lista = head; // Navega para o seguinte
        }
    }
}

/* ----------------------- Verificações ----------------------- */

/**
 * Verifica se os dados de um dado parque são válidos,
 * e destrói o parque se não o for.
 *
 * @param parque Ponteiro para estrutura Parque.
 * @return O resultado da verificação.
 */
int validaParque(Parque *parque) {
    /* Se a capacidade não for válida, devolve 0 */
    if (parque->capacidade <= 0) {
        printf("%d: invalid capacity.\n", parque->capacidade);
        destroiParque(parque);
        return FALSE;
    }
    /* Se as tarífas não forem válidas, devolve 0 */
    else if  (parque->valor_15 <= 0 || parque->valor_apos_15 <= 0
    || parque->max_diario <= 0 || parque->valor_15 >= parque->valor_apos_15
    || parque->valor_apos_15 >= parque->max_diario) {
        printf("invalid cost.\n");
        destroiParque(parque);
        return FALSE;
    }
    /* Se chegar aqui, é válido */
    else
        return TRUE;
}

/**
 * Verifica se o nome do parque que se pretende já existe ou não.
 * 
 * @param parque O parque que pretendemos criar.
 * @param verificar O parque a comparar com.
 * @return O resultado da verificação.
 */
int duplicado(Parque *parque, Parque *verificar) {
    if (!strcmp(parque->nome, verificar->nome)) {
        printf("%s: parking already exists.\n", head->nome);
        return TRUE; // Se já existir
    }
    else
        return FALSE;
}

/* ----------------------- Outputs ----------------------- */

/**
 * Lista todos os parques existentes no sistema.
 * 
 * Comando 'p'.
 */
void listaParques() {
    if (lista != NULL) { // Se a lista existir

        head = lista; // Inicializa o head no primeiro parque
        while(head != NULL) { // Escreve todos os parques existentes
            printf( "%s %d %d\n", head->nome, head->capacidade, head->livres);
            head = head->next;
        }
    }
}

/**
 * Lista todos os parques existentes no sistema após um deles ser apagado.
 * 
 * Comando 'r'.
 */
void listaParques_apagado() {
    Parque **final = (Parque**) malloc(sizeof(Parque) * MAX_P);
    int tamanho, i = 0;
    if (lista != NULL) { // Se a lista existir

        head = lista; // Inicializa o head no primeiro parque
        while(head != NULL) { // Escreve todos os parques existentes
            final[i] = head;
            head = head->next;
            i++;
        }
        tamanho = i;
        bubbleParques(final, tamanho); // Ordena

        for (i = 0; i < tamanho; i++) {
            printf("%s\n", final[i]->nome);
        }
    }
    free(final);
}

/**
 * Escreve a informação sobre um dado parque.
 *
 * Escreve o nome do parque, e o número de lugares livres.
 */
void escreveParque(Parque *parque) {
    printf("%s %d\n", parque->nome, parque->livres);
}

/* ----------------------- Funções ----------------------- */

/**
 * FUNÇÃO AUXILIAR - Troca a posição de dois dados parques num vetor.
 * 
 * @param p1 Ponteiro para pointeiro para o primeiro Parque.
 * @param p2 Ponteiro para pointeiro para o segundo Parque.
 */
void trocaParques(Parque **p1, Parque **p2) {
    Parque *auxiliar = *p2;
    *p2 = *p1;
    *p1 = auxiliar;
}

/**
 * Compara duas estruturas Parque tendo em conta os seus nomes.
 * 
 * @param p1 Ponteiro para a primeira estrutura Parque.
 * @param p2 Ponteiro para a segunda estrutura Parque.
 * @return TRUE se p1 for maior do que p2, FALSE em caso contrário.
 */
int comparaParques(Parque *p1, Parque *p2) {
    int t1 = strlen(p1->nome);
    int t2 = strlen(p2->nome), i;
    int tmenor = (t1 < t2) ? t1 : t2;

    for (i = 0; i < tmenor; i++) {
        if (p2->nome[i] == p1->nome[i])
            continue;
        else
            return p2->nome[i] > p1->nome[i];
    }
    return FALSE; // Se chegar aqui, são iguais
}

/**
 * Ordena um vetor de parques de acordo com os seus nomes.
 * 
 * Utiliza o BubbleSort.
 * 
 * @param lista Ponteiro para ponteiro para o vetor de parques.
 * @param tamanho Tamanho do vetor a ordenar.
 */
void bubbleParques(Parque **lista, int tamanho) {
    int ordenado, i, j;

    for (i = 0; i < tamanho; i++) {
        ordenado = TRUE;
        for (j = 0; j + 1 < tamanho; j++) {
            if (!comparaParques(lista[j], lista[j + 1])) {
                trocaParques(&lista[j], &lista[j + 1]); // Trocam-se os registos
                ordenado = FALSE;
            }
        }
        if (ordenado)
            break;
    }
}

/**
 * Verifica se um dado parque existe no sistema, com lugares 
 * disponíveis. 
 *
 * @param nome Nome do parque que queremos procurar.
 * @return TRUE se o parque existir e tiver lugares disponíveis, 
 * FALSE em caso contrário.
 */
int existeLugarParque(char nome[]) {
    if (lista == NULL) {
        printf("%s: no such parking.\n", nome);
        return FALSE;
    }
    else {
        head = lista;
        /* Enquanto não encontar um parque com esse nome */
        while (strcmp(head->nome, nome)) {
            if (head->next == NULL) // Se chegar ao fim da lista
                break;
            head = head->next;
        }
        if (!strcmp(head->nome, nome)) {
            if (head->livres > 0) {
                return TRUE; // Encontrou o parque
            }
            else {
                printf("%s: parking is full.\n", head->nome);
                return FALSE;
            }
        }
    }
    printf("%s: no such parking.\n", nome);
    return FALSE;
}

/**
 * Verifica se um dado parque existe no sistema. 
 *
 * @param nome Nome do parque que queremos procurar.
 * @return TRUE se o parque existir, FALSE em caso contrário.
 */
int existeParque(char nome[]) {
    if (lista == NULL) {
        printf("%s: no such parking.\n", nome);
        return FALSE;
    }
    else {
        head = lista;
        /* Enquanto não encontar um parque com esse nome */
        while (strcmp(head->nome, nome)) {
            if (head->next == NULL) // Se chegar ao fim da lista
                break;
            head = head->next;
        }
        if (!strcmp(head->nome, nome)) {
            return TRUE; // Encontrou o parque
        }
    }
    printf("%s: no such parking.\n", nome);
    return FALSE;
}

/**
 * Procura um dado parque pelo nome, e verifica se tem lugares livres. 
 *
 * @param nome Nome do parque que queremos procurar.
 * @return Ponteiro para o Parque, se ele existir, NULL em 
 * caso contrário.
 */
Parque *procuraParque(char nome[]) {
    if (lista == NULL) { // Se não houver lista de parques
        return NULL;
    }
    else {
        head = lista;
        /* Enquanto não encontar um parque com esse nome */
        while (strcmp(head->nome, nome))
            head = head->next;

        if (!strcmp(head->nome, nome)) {
            if (head->livres > 0) { // Se tiver lugares livres
                return head; // Devolve um pointer para o parque
            }
            else {
                return NULL;
            }
        }
    }
    return NULL;
}

/* ----------------------- Comandos ----------------------- */

/**
 * Cria um novo parque.
 * 
 * Comando 'p'.
 *
 * @return O resultado da criação do parque.
 */
int criaParque() {
    Parque *parque = (Parque*) calloc(1, sizeof(Parque));
    if (leParque(parque)) {
        parque->livres = parque->capacidade;
        parque->next = NULL;
    }
    else
        return FALSE;
    parque->fatura = inicialiazaFaturacao(); // Inicialiaza a lista de faturações

    if (lista == NULL && validaParque(parque)) { // Se ainda não existir lista, cria-a
        lista = parque;
        CONTADOR++;
        return TRUE;
    }
    head = lista;
    if (duplicado(parque, head)) {
        destroiParque(parque); return FALSE; // Se der erro, destroi o parque
    }
    while (head->next != NULL) {
        head = head->next;
        if (duplicado(parque, head)) {
            destroiParque(parque); return FALSE;
        }
    }
    if (CONTADOR == MAX_P) {
        printf("too many parks.\n");
        destroiParque(parque); return FALSE;
    }
    if (validaParque(parque)) {
        head->next = parque;
        CONTADOR++; return TRUE;
    }
    return FALSE;
}

/**
 * Apaga um nó do meio da lista de parques.
 * 
 * @param head Ponteiro para ponteira para o nó anterior ao a apagar.
 */
void apagaNode(Parque **head) {
    Parque *anterior = *head;
    Parque *atual = *head;
    
    atual = atual->next; // Apontamos para o que queremos apagar
    anterior->next = atual->next; // Anterior aponta para o seguinte do que queremos apagar
    destroiParque(atual);
    atual = NULL;
}

/**
 * Apaga um dado parque do sistema.
 *
 * @param pnome O nome do parque a ser apagado.
 * @return Retorna TRUE se o parque for apagado, FALSE em caso contrário.
 */
int apagaParque(char *pnome) {
    int apagou = FALSE;
    if (lista != NULL) {
        head = lista;
        while(head != NULL) { // Quando o seguinte for o que queremos apagar
            if (head->next != NULL) {   
                if (!strcmp(pnome, head->next->nome)) {
                    apagaNode(&head);
                    apagou = TRUE;
                }
            }
            head = head->next;
        }
        /* Se percorre tudo, verifica se era o primeiro elemento */
        if (!apagou && !strcmp(pnome, lista->nome)) {
            head = lista;
            lista = lista->next;
            destroiParque(head);
            apagou = TRUE;
            head = NULL;
        }
        return apagou;
    }
    return apagou;
}

/* ----------------------- Faturação dos Parques ----------------------- */

/**
 * Inicializa uma faturação e os seus parâmetros.
 *
 * @return Ponteiro para a estrutura faturação inicializada.
 */
Faturacao *inicialiazaFaturacao() {
    Faturacao *faturacao = (Faturacao*) calloc(1, sizeof(Faturacao));
    Data *pdata = (Data*) calloc(1, sizeof(Data));
    faturacao->data = pdata; 
    faturacao->total = VAZIO;
    faturacao->next = NULL;
    return faturacao;
}

/**
 * Destrói uma estrutura Faturação.
 *
 * @param faturacao Ponteiro para estrutura Faturação.
 */
void destroiFaturacao(Faturacao *faturacao) {
    free(faturacao->data);
    free(faturacao);
}

/**
 * Destrói a lista de faturações.
 *
 * @param faturcao Ponteiro para a ponteiro para o primeiro elemento da lista de faturações.
 */
void destroiListaFaturacao(Faturacao **faturcao) {
    Faturacao *current = *faturcao;
    while (current != NULL) {
        Faturacao *proximo = current->next;
        destroiFaturacao(current);
        current = proximo;
    }
    *faturcao = NULL;
}