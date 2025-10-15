#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include "funcoes.h"

/// Processa uma célula vizinha de uma posição marcada com '#'.
/// Se a célula contiver uma letra minúscula, converte-a para maiúscula,
/// regista a alteração na pilha de Undo e atualiza o identificador de push.
void processa_vizinho(int ni, int nj, int coluna, char tab[][coluna], Undo *topo, int *push_id) {
    char original = tab[ni][nj];
    char convertido = toupper(original);
    if (original != convertido) {
        push_undo(topo, ni, nj, original, *push_id);
        (*push_id)++;
        tab[ni][nj] = convertido;
    }
}

/// Verifica todas as células marcadas com '#' no tabuleiro.
/// Para cada uma, tenta converter os seus vizinhos (cima, baixo, esquerda, direita)
/// de minúscula para maiúscula, se aplicável.
void verifica_riscadas(int linha, int coluna, char tab[linha][coluna], Undo *topo, int *push_id) {
    for (int i = 0; i < linha; i++) {
        for (int j = 0; j < coluna; j++) {
            if (tab[i][j] == '#') {
                int dr[4] = {-1, 1, 0, 0};
                int dc[4] = {0, 0, -1, 1};

                for (int d = 0; d < 4; d++) {
                    int ni = i + dr[d];
                    int nj = j + dc[d];

                    if (ni >= 0 && ni < linha && nj >= 0 && nj < coluna) {
                        processa_vizinho(ni, nj, coluna, tab, topo, push_id);
                    }
                }
            }
        }
    }
}

/// Executa a riscagem horizontal e vertical a partir de uma letra maiúscula na posição (i, j).
/// Todas as letras minúsculas iguais na mesma linha ou coluna são substituídas por '#'
/// e as alterações são registadas na pilha de Undo.
void riscagem_horizontal_vertical(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas], int i, int j, int *push_id, int *alterado) {
    char letra = tolower(tabuleiro[i][j]);

    for (int k = 0; k < colunas; k++) {
        if (tabuleiro[i][k] == letra) {
            push_undo(topo, i, k, tabuleiro[i][k], (*push_id)++);
            tabuleiro[i][k] = '#';
            *alterado = 1;
        }
    }

    for (int k = 0; k < linhas; k++) {
        if (tabuleiro[k][j] == letra) {
            push_undo(topo, k, j, tabuleiro[k][j], (*push_id)++);
            tabuleiro[k][j] = '#';
            *alterado = 1;
        }
    }
}

/// Aplica o comando 'a' (riscar letras correspondentes a maiúsculas).
/// Enquanto houver alterações, repete a riscagem para letras maiúsculas.
/// Após isso, verifica também os vizinhos de células riscadas ('#') para conversões.
/// Retorna o novo índice de alterações para o Undo.
int cmd_a(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas], int ind) {
    int alterado = 1;
    int push_id = ind;

    while (alterado) {
        alterado = 0;

        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (isupper(tabuleiro[i][j])) {
                    riscagem_horizontal_vertical(topo, linhas, colunas, tabuleiro, i, j, &push_id, &alterado);
                }
            }
        }
    }

    verifica_riscadas(linhas, colunas, tabuleiro, topo, &push_id);
    return push_id;
}

/// Verifica se houve alguma mudança entre dois tabuleiros.
/// Retorna 1 se houve diferença e 0 se são idênticos.
int tabuleiromuda(int linhas, int colunas, char t1[linhas][colunas], char t2[linhas][colunas]) {
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            if (t1[i][j] != t2[i][j])
                return 1; // mudou
    return 0; // não mudou
}

/// Variante "grande" do comando 'a': aplica 'cmd_a' repetidamente até o tabuleiro estabilizar.
/// Só para quando não há mais alterações.
/// Retorna o novo índice de alterações para o Undo ou 0 se nada foi alterado.
int cmd_a_grande(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas], int ind){
    char tabuleiro2[linhas][colunas];
    int ind2, res = 0;
    do {
        for (int i = 0; i < linhas; i++)
            for (int j = 0; j < colunas; j++)
                tabuleiro2[i][j] = tabuleiro[i][j];
        ind2 = ind;
        ind = cmd_a(topo,linhas, colunas, tabuleiro, ind2);
        res++;
    } while(tabuleiromuda(linhas, colunas, tabuleiro, tabuleiro2));
    if (res==1) return 0;
    return ind;
}
