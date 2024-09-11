#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estruturas/TabelaHash.h"

#define MAX_LINHA 1024
#define MAX_BUSCA 300

// Função para inserir um novo RRN no vetor de uma palavra
void insert_rrn(HashEntry *entry, long rrn) {
    // Se o vetor de RRNs estiver cheio, dobrar a capacidade
    if (entry->count == entry->capacity) {
        entry->capacity *= 2;
        entry->rrns = (long *)realloc(entry->rrns, entry->capacity * sizeof(long));
        if (entry->rrns == NULL) {
            printf("Erro ao realocar memória.\n");
            exit(1);
        }
    }
    // Inserir o novo RRN no vetor
    entry->rrns[entry->count++] = rrn;
}

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

int main()
{

    // Abre o arquivo de tweets
    FILE *tweets = fopen("input.csv", "r");
    if (tweets == NULL)
    {
        printf("Erro ao abrir arquivo.");
        exit(1);
    }

    char linha[MAX_LINHA];               // Buffer para cada linha do arquivo
    fgets(linha, sizeof(linha), tweets); // Ignora o header

    int i = 0;
    long rrn;

    // Lê as primeiras 100 linhas do arquivo
    while (fgets(linha, sizeof(linha), tweets) && i < 5)
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
                printf("%s\n", word); // Exibe cada palavra processada
                // Inserir a palavra na tabela hash aqui
                word = strtok(NULL, " ");
            }
        }

        i++;
    }

    fclose(tweets);

    return 0;
}