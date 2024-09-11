#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TabelaHash.h"

void insert_rrn(HashEntry *entry, long rrn) {
    if (entry->count == entry->capacity) {
        entry->capacity *= 2;
        entry->rrns = (long *)realloc(entry->rrns, entry->capacity * sizeof(long));
        if (entry->rrns == NULL) {
            printf("Erro ao realocar memória.\n");
            exit(1);
        }
    }
    entry->rrns[entry->count++] = rrn;
}


Hash* criaHash(int TABLE_SIZE) {
    Hash* ha = (Hash*) malloc(sizeof(Hash));
    if (ha != NULL) {
        int i;
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->itens = (HashEntry**) malloc(TABLE_SIZE * sizeof(HashEntry*));
        if (ha->itens == NULL) {
            free(ha);
            return NULL;
        }
        ha->qtd = 0;
        for (i = 0; i < ha->TABLE_SIZE; i++)
            ha->itens[i] = NULL;
    }
    return ha;
}

void liberaHash(Hash* ha) {
    if (ha != NULL) {
        int i;
        for (i = 0; i < ha->TABLE_SIZE; i++) {
            if (ha->itens[i] != NULL) {
                free(ha->itens[i]->rrns);
                free(ha->itens[i]);
            }
        }
        free(ha->itens);
        free(ha);
    }
}

int insereHash_Palavra(Hash* ha, char *palavra, long rrn) {
    if (ha == NULL || ha->qtd == ha->TABLE_SIZE)
        return 0;

    int chave = valorString(palavra);
    int pos = chaveDivisao(chave, ha->TABLE_SIZE);

    for (int i = 0; i < ha->TABLE_SIZE; i++) {
        int newPos = sondagemLinear(pos, i, ha->TABLE_SIZE);
        if (ha->itens[newPos] == NULL) {
            HashEntry* novo = (HashEntry*) malloc(sizeof(HashEntry));
            if (novo == NULL)
                return 0;
            strcpy(novo->word, palavra);
            novo->rrns = (long*) malloc(sizeof(long) * 10);  // Capacidade inicial para 10 RRNs
            if (novo->rrns == NULL) {
                free(novo);
                return 0;
            }
            novo->rrns[0] = rrn;
            novo->count = 1;
            novo->capacity = 10;
            ha->itens[newPos] = novo;
            ha->qtd++;
            return 1;
        } else if (strcmp(ha->itens[newPos]->word, palavra) == 0) {
            insert_rrn(ha->itens[newPos], rrn);
            return 1;
        }
    }
    return 0;
}

// Função hash para strings
int valorString(char *str) {
    int i, valor = 7;
    int tam = strlen(str);
    for (i = 0; i < tam; i++)
        valor = 31 * valor + (int) str[i];
    return (valor & 0x7FFFFFFF);
}

// Hash Divisão
int chaveDivisao(int chave, int TABLE_SIZE) {
    return (chave & 0x7FFFFFFF) % TABLE_SIZE;
}

// Sondagem Linear
int sondagemLinear(int pos, int i, int TABLE_SIZE) {
    return ((pos + i) & 0x7FFFFFFF) % TABLE_SIZE;
}

// Busca
int buscaHash_Palavra(Hash* ha, char *palavra, HashEntry** entry) {
    if (ha == NULL)
        return 0;

    int chave = valorString(palavra);
    int pos = chaveDivisao(chave, ha->TABLE_SIZE);

    for (int i = 0; i < ha->TABLE_SIZE; i++) {
        int newPos = sondagemLinear(pos, i, ha->TABLE_SIZE);
        if (ha->itens[newPos] == NULL)
            return 0;

        if (strcmp(ha->itens[newPos]->word, palavra) == 0) {
            *entry = ha->itens[newPos];
            return 1;
        }
    }
    return 0;
}
