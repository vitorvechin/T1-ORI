#ifndef TABELA_HASH_H
#define TABELA_HASH_H

// Definição da estrutura HashEntry
typedef struct HashEntry {
    char word[100];
    long *rrns;      // Vetor de RRNs
    int count;       // Número de RRNs armazenados
    int capacity;    // Capacidade do vetor
} HashEntry;

// Definição da estrutura Hash
typedef struct hash Hash;

// Função para criar uma nova tabela hash
Hash* criaHash(int TABLE_SIZE);

// Função para liberar a memória utilizada pela tabela hash
void liberaHash(Hash* ha);

// Função para calcular o valor hash de uma string
int valorString(char *str);

// Função para inserir uma palavra na tabela hash (com tratamento de colisões)
int insereHash_Palavra(Hash* ha, char *palavra, long rrn);

// Função para buscar uma palavra na tabela hash e retornar a entrada correspondente
int buscaHash_Palavra(Hash* ha, char *palavra, HashEntry** entry);

// Funções para hash com diferentes métodos de sondagem (não utilizadas nesta versão, mas mantidas para referência)
// int chaveDivisao(int chave, int TABLE_SIZE);
// int chaveDobra(int chave, int TABLE_SIZE);
// int chaveMultiplicacao(int chave, int TABLE_SIZE);
// int sondagemLinear(int pos, int i, int TABLE_SIZE);
// int sondagemQuadratica(int pos, int i, int TABLE_SIZE);
// int duploHashing(int pos, int chave, int i, int TABLE_SIZE);

#endif // TABELA_HASH_H
