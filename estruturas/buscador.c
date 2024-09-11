#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buscador.h"

// Função auxiliar para substituir strndup (se necessário)
char* minha_strndup(const char* str, size_t n) {
    char* result;
    size_t len = strlen(str);
    
    if (len > n) {
        len = n;
    }
    
    result = (char*)malloc(len + 1);  // +1 para o terminador nulo
    if (!result) {
        return NULL;  // Falha ao alocar memória
    }
    
    result[len] = '\0';  // Adiciona o terminador nulo
    return memcpy(result, str, len);
}

// Define a precedência dos operadores
int precedencia(char* operador) {
    if (strcmp(operador, "NOT") == 0) return 3;
    if (strcmp(operador, "AND") == 0) return 2;
    if (strcmp(operador, "OR") == 0) return 1;
    return 0;
}

// Função que quebra a expressão em tokens
char** tokenizar(const char* expressao) {
    char** tokens = malloc(MAX_TOKENS * sizeof(char*));
    int indice_token = 0;
    char buffer[256];
    int indice_buffer = 0;

    for (int i = 0; expressao[i] != '\0'; i++) {
        char ch = expressao[i];

        if (ch == ' ') {
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0';
                tokens[indice_token++] = strdup(buffer);
                indice_buffer = 0;
            }
            continue;
        }

        if (ch == '(' || ch == ')') {
            if (indice_buffer > 0) {
                buffer[indice_buffer] = '\0';
                tokens[indice_token++] = strdup(buffer);
                indice_buffer = 0;
            }
            tokens[indice_token++] = minha_strndup(&ch, 1);
        } else {
            buffer[indice_buffer++] = ch;
        }
    }

    if (indice_buffer > 0) {
        buffer[indice_buffer] = '\0';
        tokens[indice_token++] = strdup(buffer);
    }

    tokens[indice_token] = NULL; // Fim dos tokens
    return tokens;
}

// Função que converte a expressão infixa para postfix
char** infixa_para_postfix(char** tokens) {
    char** saida = malloc(MAX_TOKENS * sizeof(char*));
    char* pilha[MAX_TOKENS];
    int indice_saida = 0, indice_pilha = 0;

    for (int i = 0; tokens[i] != NULL; i++) {
        char* token = tokens[i];

        if (strcmp(token, "(") == 0) {
            pilha[indice_pilha++] = token;
        } else if (strcmp(token, ")") == 0) {
            while (indice_pilha > 0 && strcmp(pilha[indice_pilha - 1], "(") != 0) {
                saida[indice_saida++] = pilha[--indice_pilha];
            }
            indice_pilha--; // Remove o '(' da pilha
        } else if (strcmp(token, "AND") == 0 || strcmp(token, "OR") == 0 || strcmp(token, "NOT") == 0) {
            while (indice_pilha > 0 && precedencia(pilha[indice_pilha - 1]) >= precedencia(token)) {
                saida[indice_saida++] = pilha[--indice_pilha];
            }
            pilha[indice_pilha++] = token;
        } else {
            saida[indice_saida++] = token; // Operando (palavra-chave)
        }
    }

    while (indice_pilha > 0) {
        saida[indice_saida++] = pilha[--indice_pilha];
    }

    saida[indice_saida] = NULL; // Fim da expressão
    return saida;
}

// Função que avalia a expressão postfix e retorna os RRNs desejados
VetorRRNs* avaliar_postfix(Hash* tabela, char** tokens_postfix) {
    VetorRRNs* pilha[MAX_TOKENS];
    int indice_pilha = 0;

    for (int i = 0; tokens_postfix[i] != NULL; i++) {
        char* token = tokens_postfix[i];

        if (strcmp(token, "AND") == 0) {
            VetorRRNs* direita = pilha[--indice_pilha];
            VetorRRNs* esquerda = pilha[--indice_pilha];

            // Verifica se o próximo token é "NOT"
            if (tokens_postfix[i + 1] != NULL && strcmp(tokens_postfix[i + 1], "NOT") == 0) {
                i++;  // Pula o "NOT"
                direita = subtrair_rrns(obter_todos_rrns(tabela), direita);
            }

            // Faz a interseção entre os dois conjuntos (esquerda E direita)
            pilha[indice_pilha++] = intersecao_rrns(esquerda, direita);

        } else if (strcmp(token, "OR") == 0) {
            VetorRRNs* direita = pilha[--indice_pilha];
            VetorRRNs* esquerda = pilha[--indice_pilha];

            // Verifica se o próximo token é "NOT"
            if (tokens_postfix[i + 1] != NULL && strcmp(tokens_postfix[i + 1], "NOT") == 0) {
                i++;  // Pula o "NOT"
                direita = subtrair_rrns(obter_todos_rrns(tabela), direita);
            }

            // Faz a união entre os dois conjuntos (esquerda OU direita)
            pilha[indice_pilha++] = uniao_rrns(esquerda, direita);

        } else if (strcmp(token, "NOT") == 0) {
            VetorRRNs* operando = pilha[--indice_pilha];
            pilha[indice_pilha++] = subtrair_rrns(obter_todos_rrns(tabela), operando);
        } else {
            pilha[indice_pilha++] = buscar_rrns(tabela, token);
        }
    }

    return pilha[0];  // Retorna o resultado final
}

// Função auxiliar para verificar se um RRN já existe no resultado
int rrn_existe(long* rrns, int tamanho, long rrn) {
    for (int i = 0; i < tamanho; i++) {
        if (rrns[i] == rrn) {
            return 1;  // O RRN já existe na lista
        }
    }
    return 0;  // O RRN não existe na lista
}

// Função de interseção de RRNs (E) que evita duplicações
VetorRRNs* intersecao_rrns(VetorRRNs* vetor1, VetorRRNs* vetor2) {
    VetorRRNs* resultado = malloc(sizeof(VetorRRNs));
    resultado->rrns = malloc(vetor1->tamanho * sizeof(long));  // O tamanho máximo será o do menor vetor
    resultado->tamanho = 0;

    for (int i = 0; i < vetor1->tamanho; i++) {
        for (int j = 0; j < vetor2->tamanho; j++) {
            if (vetor1->rrns[i] == vetor2->rrns[j]) {
                // Adiciona ao resultado apenas se não for uma duplicata
                if (!rrn_existe(resultado->rrns, resultado->tamanho, vetor1->rrns[i])) {
                    resultado->rrns[resultado->tamanho++] = vetor1->rrns[i];
                }
                break;
            }
        }
    }

    return resultado;
}

// Função de união de RRNs (OU)
VetorRRNs* uniao_rrns(VetorRRNs* vetor1, VetorRRNs* vetor2) {
    VetorRRNs* resultado = malloc(sizeof(VetorRRNs));
    resultado->rrns = malloc((vetor1->tamanho + vetor2->tamanho) * sizeof(long));
    resultado->tamanho = 0;

    // Adiciona todos os elementos do vetor1 ao resultado, sem duplicatas
    for (int i = 0; i < vetor1->tamanho; i++) {
        if (!rrn_existe(resultado->rrns, resultado->tamanho, vetor1->rrns[i])) {
            resultado->rrns[resultado->tamanho++] = vetor1->rrns[i];
        }
    }

    // Adiciona todos os elementos do vetor2 ao resultado, sem duplicatas
    for (int j = 0; j < vetor2->tamanho; j++) {
        if (!rrn_existe(resultado->rrns, resultado->tamanho, vetor2->rrns[j])) {
            resultado->rrns[resultado->tamanho++] = vetor2->rrns[j];
        }
    }

    return resultado;
}

// Função de subtração de RRNs (NOT)
VetorRRNs* subtrair_rrns(VetorRRNs* vetor_completo, VetorRRNs* vetor_para_excluir) {
    // Verifica se os vetores de entrada são válidos
    if (vetor_completo == NULL || vetor_completo->rrns == NULL || vetor_completo->tamanho == 0) {
        printf("Erro: vetor_completo é NULL ou vazio.\n");
        return NULL;  // Se o vetor completo for inválido, não há nada a subtrair
    }
    
    // Se o vetor para excluir for inválido ou vazio, retorna o vetor completo
    if (vetor_para_excluir == NULL || vetor_para_excluir->rrns == NULL || vetor_para_excluir->tamanho == 0) {
        printf("Aviso: vetor_para_excluir é NULL ou vazio. Retornando vetor_completo.\n");
        VetorRRNs* resultado = malloc(sizeof(VetorRRNs));
        resultado->rrns = malloc(vetor_completo->tamanho * sizeof(long));
        memcpy(resultado->rrns, vetor_completo->rrns, vetor_completo->tamanho * sizeof(long));
        resultado->tamanho = vetor_completo->tamanho;
        return resultado;
    }

    // Cria o resultado inicial com espaço suficiente
    VetorRRNs* resultado = malloc(sizeof(VetorRRNs));
    if (resultado == NULL) {
        printf("Erro: Falha ao alocar memória para resultado.\n");
        exit(1);
    }
    
    resultado->rrns = malloc(vetor_completo->tamanho * sizeof(long));  // O tamanho máximo será o do vetor completo
    if (resultado->rrns == NULL) {
        printf("Erro: Falha ao alocar memória para resultado->rrns.\n");
        free(resultado);
        exit(1);
    }
    
    resultado->tamanho = 0;

    // Percorre o vetor completo e subtrai os elementos que estão no vetor para excluir
    for (int i = 0; i < vetor_completo->tamanho; i++) {
        int encontrado = 0;
        for (int j = 0; j < vetor_para_excluir->tamanho; j++) {
            if (vetor_completo->rrns[i] == vetor_para_excluir->rrns[j]) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            // Adiciona ao resultado se o elemento não foi encontrado no vetor_para_excluir
            resultado->rrns[resultado->tamanho++] = vetor_completo->rrns[i];
        }
    }

    // Verifica se algum resultado foi adicionado, caso contrário, libera a memória
    if (resultado->tamanho == 0) {
        free(resultado->rrns);
        free(resultado);
        printf("Aviso: Nenhum RRN restante após a subtração.\n");
        return NULL;
    }

    return resultado;
}

// Função para buscar os RRNs associados a uma palavra na tabela hash
VetorRRNs* buscar_rrns(Hash* tabela, const char* palavra) {
    int i;
    // Percorre a tabela procurando pela palavra
    for (i = 0; i < tabela->TABLE_SIZE; i++) {
        if (tabela->itens[i] != NULL && strcmp(tabela->itens[i]->word, palavra) == 0) {
            // Palavra encontrada, retorna os RRNs
            VetorRRNs* resultado = malloc(sizeof(VetorRRNs));
            resultado->rrns = malloc(tabela->itens[i]->count * sizeof(long));  // RRNs são long
            
            // Copia os RRNs para o vetor de resultado
            for (int j = 0; j < tabela->itens[i]->count; j++) {
                resultado->rrns[j] = tabela->itens[i]->rrns[j];
            }
            resultado->tamanho = tabela->itens[i]->count;
            return resultado;
        }
    }

    // Se a palavra não for encontrada, retorna um vetor vazio
    VetorRRNs* vazio = malloc(sizeof(VetorRRNs));
    vazio->rrns = NULL;
    vazio->tamanho = 0;
    return vazio;
}

// Função para obter todos os RRNs da tabela hash
VetorRRNs* obter_todos_rrns(Hash* tabela) {
    VetorRRNs* resultado = malloc(sizeof(VetorRRNs));
    resultado->rrns = malloc(1000 * sizeof(long));  // Supondo um máximo de 1000 RRNs (ajuste conforme necessário)
    resultado->tamanho = 0;

    // Itera sobre a tabela e coleta todos os RRNs
    for (int i = 0; i < tabela->TABLE_SIZE; i++) {
        if (tabela->itens[i] != NULL) {
            for (int j = 0; j < tabela->itens[i]->count; j++) {
                resultado->rrns[resultado->tamanho++] = tabela->itens[i]->rrns[j];
            }
        }
    }
    
    return resultado;
}
