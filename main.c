#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estruturas/TabelaHash.h"

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
    FILE *tweets = fopen("input.csv", "r");
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
    while (fgets(linha, sizeof(linha), tweets) && i < 100)
    {
        rrn = ftell(tweets) - strlen(linha);  // Armazena o deslocamento do início da linha
        printf("RRN: %ld\n", rrn);

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
                printf("Inserindo palavra: %s\n", word); // Exibe cada palavra processada
                insereHash_Palavra(hash, word, rrn); // Insere a palavra na tabela hash
                word = strtok(NULL, " ");
            }
        }

        i++;
    }

    fclose(tweets);

    // Teste de busca
    char palavra[MAX_BUSCA];
    printf("Digite uma palavra para buscar: ");
    fgets(palavra, sizeof(palavra), stdin);
    palavra[strcspn(palavra, "\n")] = '\0'; // Remove o newline do fgets

    buscaEExibeTweets(hash, palavra);

    liberaHash(hash); // Libera a memória alocada para a tabela hash

    return 0;
}
