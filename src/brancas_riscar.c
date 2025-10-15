#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <stdlib.h>
#include <dirent.h>
#include "funcoes.h"

int coordenada(int linhas, int colunas, char tabuleiro[linhas][colunas], char s[]) {
    if (strlen(s) < 4) {
        printf("Coordenada fora do tabuleiro.\n");
        return 1;
    }

    int c = s[2] - 'a'; // Coluna
    int l = atoi(&s[3]) - 1; // Linha

    if (c < 0 || c >= colunas || l < 0 || l >= linhas) {
        printf("Coordenada fora do tabuleiro.\n");
        return 1;
    }

    printf("Coordenada da linha %d e coluna %d: %c\n", l + 1, c + 1, tabuleiro[l][c]);
    return 0;
}

int cmd_b(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas], char s[], int ind) {
    int c = s[2] - 'a';
    int l = atoi(&s[3]) - 1;

    // Adiciona verificação se a coordenada é válida
    if (c < 0 || c >= colunas || l < 0 || l >= linhas) 
        return 0;

    if (islower(tabuleiro[l][c])) {
        // Salva o estado atual antes de modificar a célula
        push_undo(topo, l, c, tabuleiro[l][c], ind);

        // Modifica a célula (pinta para maiúscula)
        tabuleiro[l][c] = toupper(tabuleiro[l][c]);
        return 1;
    } else 
        return 0;
}

int cmd_r(Undo *topo,int linhas, int colunas, char tabuleiro[linhas][colunas], char s[], int ind) {
    int c = s[2] - 'a'; // Calcula a coluna (letra)
    int l = atoi(&s[3]) - 1; // Calcula a linha (número)

    // Adiciona verificação se a coordenada é válida
    if (c < 0 || c >= colunas || l < 0 || l >= linhas) 
        return 0;

    if (tabuleiro[l][c] != '#') {
        // Salva o estado atual antes de modificar a célula
        push_undo(topo,l, c, tabuleiro[l][c], ind);

        // Modifica a célula (risca com '#')
        tabuleiro[l][c] = '#';
        return 1;
    } else 
        return 0;
}