#ifndef TABELA_HASH_H
#define TABELA_HASH_H

typedef struct HashEntry {
    char word[100];
    long *rrns;      // Vetor de RRNs
    int count;       // Número de RRNs armazenados
    int capacity;    // Capacidade do vetor
} HashEntry;

typedef struct hash {
    int qtd, TABLE_SIZE;
    HashEntry **itens;
} Hash;

// Funções para criar e liberar a hash
Hash* criaHash(int TABLE_SIZE);
void liberaHash(Hash* ha);

// Funções para inserção e busca de palavras
int insereHash_Palavra(Hash* ha, char *palavra, long rrn);
int buscaHash_Palavra(Hash* ha, char *palavra, HashEntry** entry);

// Funções auxiliares
int valorString(char *str);
int chaveDivisao(int chave, int TABLE_SIZE);
int sondagemLinear(int pos, int i, int TABLE_SIZE);

#endif // TABELA_HASH_H
