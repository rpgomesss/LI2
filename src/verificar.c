#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <stdlib.h>
#include <dirent.h>
#include "funcoes.h"

// Função que verifica se as celulas em branco estão conectadas
void floodfill(int i, int j, int linhas, int colunas, char tabuleiro[linhas][colunas], int visitado[linhas][colunas]) {
    // Verifica se os indices estão fora dos limites do tabuleiro
    if (i < 0 || i >= linhas || j < 0 || j >= colunas) return;
    //Verifica se a célula atual não é maiúscula ou se já foi visitada 
    if (!isupper(tabuleiro[i][j]) || visitado[i][j]) return;

    visitado[i][j] = 1; // Marca a célula como visitada 

    // Chamada recursiva para as quatro direções adjacentes
    floodfill(i+1, j, linhas, colunas, tabuleiro, visitado);
    floodfill(i-1, j, linhas, colunas, tabuleiro, visitado);
    floodfill(i, j+1, linhas, colunas, tabuleiro, visitado);
    floodfill(i, j-1, linhas, colunas, tabuleiro, visitado);
}

// Verifica duplicados nas linhas
int verificar_duplicados_linhas(int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    int erros = 0; // Contador de erros encontrados
    // Percorre todas as linhas do tabuleiro
    for (int i = 0; i < linhas; i++) {
        // Percorre todo o alfabeto
        for (char letra = 'A'; letra <= 'Z'; letra++) {
            int count = 0; // Contador de ocorrências da letra na linha 
            // Percorre todas as colunas da linha atual
            for (int j = 0; j < colunas; j++) {
                if (tabuleiro[i][j] == letra) count++;
            }
            // Se a letra apareceu mais de uma vez, imprime erro e conta
            if (count > 1) {
                printf("Erro: letra '%c' repetida na linha %d\n", letra, i+1);
                erros++;
            }
        }
    }
    return erros;
}

// Verifica duplicados nas colunas
int verificar_duplicados_colunas(int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    int erros = 0;
    for (int j = 0; j < colunas; j++) {
        for (char letra = 'A'; letra <= 'Z'; letra++) {
            int count = 0;
            for (int i = 0; i < linhas; i++) {
                if (tabuleiro[i][j] == letra) count++;
            }
            if (count > 1) {
                printf("Erro: letra '%c' repetida na coluna %d\n", letra, j+1);
                erros++;
            }
        }
    }
    return erros;
}

// Verifica se casas riscadas estão cercadas apenas por brancas
int verificar_casas_riscadas(int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    int erros = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] == '#') {
                int valid = 1;
                if (i > 0 && !isupper(tabuleiro[i-1][j])) valid = 0;
                if (i < linhas - 1 && !isupper(tabuleiro[i+1][j])) valid = 0;
                if (j > 0 && !isupper(tabuleiro[i][j-1])) valid = 0;
                if (j < colunas - 1 && !isupper(tabuleiro[i][j+1])) valid = 0;

                if (!valid) {
                    printf("Erro: casa riscada em (%d,%d) não está cercada por brancas\n", i+1, j+1);
                    erros++;
                }
            }
        }
    }
    return erros;
}

// Verifica se todas as casas brancas estão conectadas
int verificar_conectividade_brancas(int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    int visitado[linhas][colunas];
    memset(visitado, 0, sizeof(visitado));
    int erros = 0;
    int found = 0;

    for (int i = 0; i < linhas && !found; i++) {
        for (int j = 0; j < colunas && !found; j++) {
            if (isupper(tabuleiro[i][j])) {
                floodfill(i, j, linhas, colunas, tabuleiro, visitado);
                found = 1;
            }
        }
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (isupper(tabuleiro[i][j]) && !visitado[i][j]) {
                printf("Erro: casa branca em (%d,%d) está isolada\n", i+1, j+1);
                erros++;
            }
        }
    }

    if (found && erros == 0) {
        printf("Todas as casas brancas estão conectadas.\n");
    }

    return erros;
}

// Função principal de validação
int cmd_v(int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    int erros = 0;

    erros += verificar_duplicados_linhas(linhas, colunas, tabuleiro);
    erros += verificar_duplicados_colunas(linhas, colunas, tabuleiro);
    erros += verificar_casas_riscadas(linhas, colunas, tabuleiro);
    erros += verificar_conectividade_brancas(linhas, colunas, tabuleiro);

    if (!erros) {
        printf("Nenhuma restrição foi violada!\n");
    }

    return erros;
}
