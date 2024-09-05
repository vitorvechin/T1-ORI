#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char *postagens[] = {
    "usa won the game but not a winner",
    "brazil celebrates carnaval every year",
    "winner of the usa contest",
    "the winner of brazil carnaval is celebrated",
    "usa celebrates victory",
    "brazil loves carnaval"
};

// Função que verifica se uma postagem contém uma palavra
bool contem_palavra(char *postagem, const char *palavra) {
    return strstr(postagem, palavra) != NULL;
}

// Função que realiza a operação lógica AND
void aplica_and(bool resultado_atual[], bool nova_condicao[], int qtd_postagens) {
    for (int i = 0; i < qtd_postagens; i++) {
        resultado_atual[i] = resultado_atual[i] && nova_condicao[i];
    }
}

// Função que realiza a operação lógica OR
void aplica_or(bool resultado_atual[], bool nova_condicao[], int qtd_postagens) {
    for (int i = 0; i < qtd_postagens; i++) {
        resultado_atual[i] = resultado_atual[i] || nova_condicao[i];
    }
}

// Função que realiza a operação lógica NOT
void aplica_not(bool resultado_atual[], const char *palavra, int qtd_postagens) {
    for (int i = 0; i < qtd_postagens; i++) {
        if (contem_palavra(postagens[i], palavra)) {
            resultado_atual[i] = false;
        }
    }
}

// Função para buscar postagens com base na palavra
void busca_palavra(bool resultado[], const char *palavra, int qtd_postagens) {
    for (int i = 0; i < qtd_postagens; i++) {
        resultado[i] = contem_palavra(postagens[i], palavra);
    }
}

// Função principal de busca
void buscar(char *consulta) {
    int qtd_postagens = sizeof(postagens) / sizeof(postagens[0]);
    bool resultado[qtd_postagens]; // Resultado final
    bool condicao_temp[qtd_postagens]; // Armazena condições intermediárias
    memset(resultado, false, sizeof(resultado)); // Inicializa resultado como falso

    char *token;
    char *operador = NULL;

    token = strtok(consulta, " ");
    if (token != NULL) {
        // Inicializa a primeira palavra no resultado
        busca_palavra(resultado, token, qtd_postagens);
    }

    // Itera sobre os operadores e palavras
    while (token != NULL) {
        token = strtok(NULL, " ");
        if (token == NULL) break;

        if (strcmp(token, "AND") == 0 || strcmp(token, "OR") == 0 || strcmp(token, "NOT") == 0) {
            operador = token;
        } else {
            // Se operador for NOT, aplica NOT diretamente
            if (operador != NULL && strcmp(operador, "NOT") == 0) {
                aplica_not(resultado, token, qtd_postagens);
            }
            // Se operador for AND, aplica AND com a nova palavra
            else if (operador != NULL && strcmp(operador, "AND") == 0) {
                busca_palavra(condicao_temp, token, qtd_postagens);
                aplica_and(resultado, condicao_temp, qtd_postagens);
            }
            // Se operador for OR, aplica OR com a nova palavra
            else if (operador != NULL && strcmp(operador, "OR") == 0) {
                busca_palavra(condicao_temp, token, qtd_postagens);
                aplica_or(resultado, condicao_temp, qtd_postagens);
            }
        }
    }

    // Exibir os resultados
    printf("Postagens encontradas:\n");
    bool encontrou = false;
    for (int i = 0; i < qtd_postagens; i++) {
        if (resultado[i]) {
            printf("- %s\n", postagens[i]);
            encontrou = true;
        }
    }

    if (!encontrou) {
        printf("Nenhuma postagem encontrada.\n");
    }
}

int main() {
    char consulta[256];

    printf("Bem-vindo ao sistema de busca de postagens!\n");
    printf("Digite sua consulta (use AND, OR, NOT para operadores lógicos):\n");

    while (1) {
        // printf("\nO que quer fazer? ");
        // printf("\n1. Inserir frase para busca\n2. Sair\n\n");

        printf("\n>>> ");
        fgets(consulta, sizeof(consulta), stdin);

        // Remover o caractere de nova linha (se existir)
        consulta[strcspn(consulta, "\n")] = 0;

        // Se o usuário digitar 'exit', o programa será encerrado
        if (strcmp(consulta, "exit") == 0) {
            printf("Encerrando o programa.\n");
            break;
        }

        // Chama a função buscar com a consulta do usuário
        buscar(consulta);
    }

    return 0;
}
