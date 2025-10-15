#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include "funcoes.h"

// Verifica unicidade e restrições locais
int check_partial(int r, int c, int L, int C, char board[L][C]) {
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    if (isupper(board[r][c])) {
        char ch = board[r][c];
        for (int i = 0; i < C; i++) {
            if (i != c && board[r][i] == ch)
                return 0;
        }
        for (int i = 0; i < L; i++) {
            if (i != r && board[i][c] == ch)
                return 0;
        }
    }

    if (board[r][c] == '#') {
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            int dentro = !(nr < 0 || nr >= L || nc < 0 || nc >= C);
            if (dentro && board[nr][nc] == '#')
                return 0;
        }
    }

    return 1;
}

// Verifica conexidade de brancas
int connected_whites(int L, int C, char board[L][C]) {
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    int vis[100][100] = {0};
    int found = 0, sr = -1, sc = -1;

    for (int i = 0; i < L && sr == -1; i++) {
        for (int j = 0; j < C; j++) {
            if (isupper(board[i][j])) {
                sr = i;
                sc = j;
                found = 1;
                j = C; // força saída do loop interno
            }
        }
    }

    if (!found)
        return 1;

    int q[10000][2], qt = 0, qh = 0;
    q[qt][0] = sr;
    q[qt++][1] = sc;
    vis[sr][sc] = 1;

    while (qh < qt) {
        int r = q[qh][0], c = q[qh][1];
        qh++;
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            int dentro = !(nr < 0 || nr >= L || nc < 0 || nc >= C);
            if (dentro && !vis[nr][nc] && isupper(board[nr][nc])) {
                vis[nr][nc] = 1;
                q[qt][0] = nr;
                q[qt++][1] = nc;
            }
        }
    }

    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            if (isupper(board[i][j]) && !vis[i][j])
                return 0;
        }
    }

    return 1;
}

// Aplica mudança e guarda no Undo
void set_cell(int r, int c, char novo, int L, int C, char board[L][C], Undo *global_undo, int *push_id) {
    push_undo(global_undo, r, c, board[r][c], *push_id);
    board[r][c] = novo;
    (*push_id)++;
}

// Backtracking
int solve_backtrack(int L, int C, char board[L][C], Undo *global_undo, int *push_id) {
    int br = -1, bc = -1;
    int encontrou = 0;

    for (int i = 0; i < L && !encontrou; i++) {
        for (int j = 0; j < C; j++) {
            if (islower(board[i][j])) {
                br = i;
                bc = j;
                encontrou = 1;
                j = C; // força saída do loop interno
            }
        }
    }

    if (!encontrou)
        return connected_whites(L, C, board);

    char original = board[br][bc];

    set_cell(br, bc, toupper(original), L, C, board, global_undo, push_id);
    if (check_partial(br, bc, L, C, board) &&
        solve_backtrack(L, C, board, global_undo, push_id)) {
        return 1;
    }

    set_cell(br, bc, '#', L, C, board, global_undo, push_id);
    if (check_partial(br, bc, L, C, board) &&
        solve_backtrack(L, C, board, global_undo, push_id)) {
        return 1;
    }

    set_cell(br, bc, original, L, C, board, global_undo, push_id);
    return 0;
}

// Função principal chamada externamente
void resolve_aux(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    int push_id = 0;
    Undo *global_undo = topo;

    char (*board)[colunas] = malloc(sizeof(char[linhas][colunas]));
    push_id = normalizar_tabuleiro(linhas, colunas, tabuleiro, topo, board);

    if (solve_backtrack(linhas, colunas, board, global_undo, &push_id)) {
        for (int i = 0; i < linhas; i++)
            for (int j = 0; j < colunas; j++)
                tabuleiro[i][j] = board[i][j];
    } else {
        fprintf(stderr, "Sem solução\n");
    }

    free(board);
}
