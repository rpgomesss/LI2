#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <stdlib.h>
#include <dirent.h>
#include "funcoes.h"

void construir_nome_ficheiro(char *destino, char *nome_base) {
    // Copia o diretório base onde os ficheiros são guardados
    strcpy(destino, "src/tabuleiros/");

    // Adiciona o nome base ao caminho
    strcat(destino, nome_base);

    // Verifica se o nome já tem a extensão ".txt"
    int len = strlen(nome_base);
    if (len < 4 || strcmp(&nome_base[len - 4], ".txt") != 0) {
        // Se não tiver, adiciona a extensão
        strcat(destino, ".txt");
    }
}

void escrever_tabuleiro(FILE *fptr, int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    // Escreve o número de linhas e colunas no ficheiro
    fprintf(fptr, "%d %d\n", linhas, colunas);

    // Percorre a matriz e escreve os caracteres do tabuleiro
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fputc(tabuleiro[i][j], fptr); 
        }
        fputc('\n', fptr); // Nova linha após cada linha do tabuleiro
    }
}

void escrever_stack(FILE *fptr, Undo topo) {
    int contador = 0;
    Undo aux = topo;

    // Conta quantos elementos existem na pilha
    while (aux != NULL) {
        contador++;
        aux = aux->prox;
    }

    // Escreve o número de elementos da pilha
    fprintf(fptr, "%d\n", contador);

    // Reinicia o auxiliar para percorrer novamente
    aux = topo;

    // Escreve cada elemento da pilha: linha, coluna, valor
    while (aux != NULL) {
        fprintf(fptr, "%d %d %c %d\n", aux->linha, aux->coluna, aux->valor, aux->ind);
        aux = aux->prox;
    }
}

void gravarjogo(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas], char *novof) {
    char nomeCompleto[150];

    // Constrói o nome completo do ficheiro com caminho e extensão
    construir_nome_ficheiro(nomeCompleto, novof);

    // Abre o ficheiro para escrita
    FILE *fptr = fopen(nomeCompleto, "w");
    if (fptr == NULL) {
        perror("Erro ao criar o ficheiro");
        return;
    }

    // Escreve o tabuleiro e a stack de jogadas no ficheiro
    escrever_tabuleiro(fptr, linhas, colunas, tabuleiro);
    escrever_stack(fptr, *topo);

    // Fecha o ficheiro
    fclose(fptr);

    // Informa o utilizador do sucesso da operação
    printf("Jogo salvo com sucesso no ficheiro '%s'.\n", novof);
}
