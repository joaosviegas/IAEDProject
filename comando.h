#ifndef _COMANDOS_
#define _COMANDOS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tempo.h"
#include "parque.h"
#include "faturacao.h"
#include "veiculo.h"
#include "registo.h"

Veiculo *criaVeiculo(Hashmap *hashmap);

int saidaVeiculo(Hashmap *hashmap);

int listaFaturacao();

int removeParque(Hashmap *hashmap);

#endif