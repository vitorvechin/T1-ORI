#ifndef BUSCADOR_H
#define BUSCADOR_H

#include "TabelaHash.h"  // Inclui a estrutura da Tabela Hash

// Definição da estrutura para armazenar os RRNs retornados
typedef struct {
    long* rrns;   // Vetor de RRNs (inteiros long)
    int tamanho;  // Número de elementos no vetor
} VetorRRNs;

#define MAX_TOKENS 100

// Funções de manipulação de tokens e expressão
char** tokenizar(const char* expressao);
char** infixa_para_postfix(char** tokens);
VetorRRNs* avaliar_postfix(Hash* tabela, char** tokens_postfix);

// Funções para manipulação de vetores de RRNs
VetorRRNs* intersecao_rrns(VetorRRNs* vetor1, VetorRRNs* vetor2);
VetorRRNs* uniao_rrns(VetorRRNs* vetor1, VetorRRNs* vetor2);
VetorRRNs* subtrair_rrns(VetorRRNs* vetor_completo, VetorRRNs* vetor_para_excluir);

// Funções para busca na tabela hash
VetorRRNs* buscar_rrns(Hash* tabela, const char* palavra);
VetorRRNs* obter_todos_rrns(Hash* tabela);

#endif // BUSCADOR_H
