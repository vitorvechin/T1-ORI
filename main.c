// Trabalho Final ORI

// Antonio Cicero Amorim de Azevedo - RA:811455
// Leonardo Triiltz Siqueira - RA: 811904
// Lucas Augusto dos Santos Anadão - RA: 811939
// Vitor Rodrigues Vechin - RA: 812287

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estruturas/TabelaHash.h"
#include "estruturas/buscador.h"

#define MAX_LINHA 1024
#define MAX_BUSCA 300
#define TABLE_SIZE 1000  // Define o tamanho da tabela hash

// Função para transformar todas as letras em minúsculas
void to_lowercase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

// Função para remover caracteres não alfanuméricos
void clean_string(char *str)
{
    int j = 0;
    for (int i = 0; str[i]; i++)
    {
        if (isalnum(str[i]) || str[i] == ' ')
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

// Função para exibir uma entrada da hash
void exibeEntrada(HashEntry *entry) {
    if (entry != NULL) {
        printf("Palavra: %s\n", entry->word);
        printf("RRNs: ");
        for (int i = 0; i < entry->count; i++) {
            printf("%ld ", entry->rrns[i]);
        }
        printf("\n");
    } else {
        printf("Entrada não encontrada.\n");
    }
}

// Função para buscar e exibir tweets associados a uma palavra
void buscaEExibeTweets(Hash *hash, char *palavra) {
    HashEntry *entry;
    if (buscaHash_Palavra(hash, palavra, &entry)) {
        printf("Tweets para a palavra '%s':\n", palavra);
        exibeEntrada(entry);
    } else {
        printf("Palavra '%s' não encontrada na hash.\n", palavra);
    }
}

int main()
{
    // Cria a tabela hash
    Hash *hash = criaHash(TABLE_SIZE);
    if (hash == NULL)
    {
        printf("Erro ao criar tabela hash.\n");
        exit(1);
    }

    // Abre o arquivo de tweets
    FILE *tweets = fopen("corpus.csv", "r");
    if (tweets == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        liberaHash(hash);
        exit(1);
    }

    char linha[MAX_LINHA];               // Buffer para cada linha do arquivo
    fgets(linha, sizeof(linha), tweets); // Ignora o header

    int i = 0;
    long rrn;

    // Lê as linhas do arquivo
    while (fgets(linha, sizeof(linha), tweets) && i < 100){
        rrn = ftell(tweets) - strlen(linha);  // Armazena o deslocamento do início da linha

        // Ignorar as duas primeiras colunas
        char *token = strtok(linha, ","); // Primeira coluna (int)
        token = strtok(NULL, ",");        // Segunda coluna (int)
        token = strtok(NULL, ",");        // Terceira coluna (tweet)

        if (token != NULL)
        {
            to_lowercase(token); // Transforma em minúsculas
            clean_string(token); // Remove caracteres não alfanuméricos

            // Aqui você pode separar as palavras do tweet e processá-las
            char *word = strtok(token, " ");
            while (word != NULL)
            {
                insereHash_Palavra(hash, word, rrn); // Insere a palavra na tabela hash
                word = strtok(NULL, " ");
            }
        }

        i++;
    }

    int opcao;
    char buffer[MAX_LINHA];
    
    while (1) {
        // Exibir o menu
        printf("Escolha uma opcao:\n");
        printf("1 - Fazer uma pesquisa\n");
        printf("2 - Sair\n");
        printf("Digite sua escolha: ");
        scanf("%d", &opcao);
        
        // Limpa o buffer do teclado após a entrada do número
        while (getchar() != '\n');

        if (opcao == 1) {
            // Defina um buffer para a expressão do usuário
            char expressao[256];  // Tamanho máximo da expressão de busca

            // Solicitar ao usuário para inserir a expressão de busca
            printf("Digite a expressao de busca: ");
            fgets(expressao, sizeof(expressao), stdin);  // Captura a entrada do usuário

            // Remove a nova linha do final da string (se presente)
            expressao[strcspn(expressao, "\n")] = 0;
            
            // Tokenizar a expressão
            char** tokens = tokenizar(expressao);
            
            // Converter para postfix
            char** postfix = infixa_para_postfix(tokens);
            
            // Avaliar a expressão
            VetorRRNs* resultado = avaliar_postfix(hash, postfix);
            
            // Exibir os resultados da busca
            if (resultado->tamanho == 0) {
                printf("Nenhum RRN encontrado.\n");
            } else {
                printf("%d resultados encontrados para a expressao '%s':\n", resultado->tamanho, expressao);
                for (int j = 0; j < resultado->tamanho; j++) {
                    fseek(tweets, resultado->rrns[j], SEEK_SET);
                    // Lê a linha correspondente
                    if (fgets(buffer, MAX_LINHA, tweets) != NULL) {
                        printf("RRN %ld: %s", resultado->rrns[j], buffer);
                    } else {
                        printf("Erro ao ler o RRN %ld no arquivo.\n", rrn);
                    }
                }
            }

            // Liberação de memória dos resultados
            free(resultado->rrns);
            free(resultado);
            
            printf("\n");

        } else if (opcao == 2) {
            // Sair do programa
            printf("Encerrando o programa...\n");
            break;
        } else {
            // Opção inválida
            printf("Opção invalida. Por favor, escolha 1 ou 2.\n");
        }
    }


    liberaHash(hash); // Libera a memória alocada para a tabela hash
    fclose(tweets); // fecha arquivo de leitura / escrita

    return 0;
}
