/* iaed24 - ist1109685 - project */

#include "comando.h"

int main() {
    char comando;
    Hashmap *hashmap = inicializaHashmap();

    while (TRUE) {

        scanf("%c", &comando);
        switch (comando) {

        case 'q':
            destroiLista();
            destroiHashmap(hashmap);
            destroiListaRegisto();
            exit(EXIT_SUCCESS);

        case 'p':
            if (getchar() == '\n') {
                listaParques();
                break;
            }
            else {
                criaParque();
                break;
            }

        case 'e':
            criaVeiculo(hashmap);
            break;

        case 's':
            saidaVeiculo(hashmap);
            break;

        case 'v':
            listaEntradas();
            break;

        case 'f':
            listaFaturacao();
            break;

        case 'r':
            removeParque(hashmap);
            break;

        case 'u':
            somaValor();
            break;
        }
    }
    return EXIT_SUCCESS;
}