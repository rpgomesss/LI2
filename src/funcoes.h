#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>

typedef struct Undo {
    int linha;
    int coluna;
    char valor;
    int ind;
    struct Undo *prox;
} *Undo;


void printmatriz(int linhas, int colunas, char tabuleiro[linhas][colunas]);

void push_undo(Undo *topo,int linha, int coluna, char valor, int ind);

int desfazer(Undo *topo,int linhas, int colunas, char tabuleiro[linhas][colunas]);

int cmd_b(Undo *topo,int linhas, int colunas, char tabuleiro[linhas][colunas], char s[], int ind);

int cmd_r(Undo *topo,int linhas, int colunas, char tabuleiro[linhas][colunas], char s[], int ind);

void gravarjogo(Undo *topo,int linhas, int colunas, char tabuleiro[linhas][colunas], char *novof);

void lerjogo(Undo *topo, char *novof, int *linhas, int *colunas, char tabuleiro[*linhas][*colunas]);

int coordenada (int linhas, int colunas, char tabuleiro[linhas][colunas], char s[]);

void floodfill(int i, int j, int linhas, int colunas, char tabuleiro[linhas][colunas], int visitado[linhas][colunas]);

int cmd_v(int linhas, int colunas, char tabuleiro[linhas][colunas]);

int scanftab_e_stack(Undo *topo,char *nomeFicheiro, int linhas, int colunas, char tabuleiro[linhas][colunas]);

int obter_dimensoes(const char *nomeFicheiro, int *linhas, int *colunas);

int cmd_a(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas], int ind);

void verifica_riscadas(int linha, int coluna, char tab[linha][coluna], Undo *topo, int *push_id);

int normalizar_tabuleiro(int linhas, int colunas, char original[linhas][colunas], Undo *topo, char tabuleiro_normalizado[linhas][colunas]);

void resolve_aux(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas]);

void cmd_resolver(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas]);

int solve_backtrack(int L, int C, char board[L][C], Undo *global_undo, int *push_id);

void set_cell(int r, int c, char novo, int L, int C, char board[L][C], Undo *global_undo, int *push_id);

int connected_whites(int L, int C, char board[L][C]);

int check_partial(int r, int c, int L, int C, char board[L][C]);

int cmd_a_grande(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas], int ind);


#endif