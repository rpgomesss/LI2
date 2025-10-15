#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <stdlib.h>
#include "funcoes.h"

int main() {
    printf("Bem-vindo ao pior jogo que existe.\n");

    Undo topo = NULL;
    char s[100];
    int linhas = 0, colunas = 0;
    int tabuleiroCarregado = 0;
    char nomeFicheiro[100];
    char nomeCompleto[150];
    char (*tabuleiro)[100] = NULL;

    // Loop até o tabuleiro ser carregado corretamente com "l <nome>"
    while (!tabuleiroCarregado) {
        printf("Use o comando l <nomeDoFicheiro> para carregar um tabuleiro ou s para sair.\n");

        if (scanf(" %[^\n]%*c", s) < 1) {
            printf("Comando inválido.\n");
        } else if (strcmp(s, "s") == 0) {
            printf("Jogo terminado\n");
            return 0;
        } else if (s[0] == 'l' && s[1] == ' ' && (isalpha(s[2]) || isdigit(s[2]))) {
            sscanf(s + 2, "%s", nomeFicheiro);
            strcpy(nomeCompleto, "src/tabuleiros/");
            strcat(nomeCompleto, nomeFicheiro);
            int len = strlen(nomeFicheiro); 
            if (len < 4 || strcmp(&nomeFicheiro[len - 4], ".txt") != 0) {
                strcat(nomeCompleto, ".txt");
            }

            if (obter_dimensoes(nomeCompleto, &linhas, &colunas)) {
                printf("Erro ao abrir o ficheiro '%s'.\n", nomeCompleto);
            } else {
                static char matriz[100][100];
                tabuleiro = matriz;

                scanftab_e_stack(&topo, nomeCompleto, linhas, colunas, tabuleiro);
                printmatriz(linhas, colunas, tabuleiro);
                tabuleiroCarregado = 1;
            }
        } else {
            printf("Comando inválido.\n");
        }
    }

    // Loop principal de comandos
    int sair = 0;
    while (!sair) {
        printf("Introduza um comando.\n");

        if (scanf(" %[^\n]%*c", s) < 1) {
            printf("Comando inválido.\n");
        } else if (strcmp(s, "s") == 0) {
            printf("Jogo terminado\n");
            sair = 1;
        } else if (s[0] == 'b' && isspace(s[1]) && islower(s[2]) && isdigit(s[3])) {
            if (cmd_b(&topo, linhas, colunas, tabuleiro, s, 0)==0) printf("Não foi possíve por em branco.\n");
            else printf("Célula %c%d pintada.\n", s[2], atoi(&s[3]));
            printmatriz(linhas, colunas, tabuleiro);
        } else if (s[0] == 'r' && isspace(s[1]) && islower(s[2]) && isdigit(s[3])) {
            if (cmd_r(&topo, linhas, colunas, tabuleiro, s, 0)==0) printf("Não foi possível riscar.\n");
            else printf("Célula %c%d riscada.\n", s[2], atoi(&s[3]));
            printmatriz(linhas, colunas, tabuleiro);
        } else if (s[0] == 'g' && isspace(s[1]) && isalpha(s[2])) {
            gravarjogo(&topo, linhas, colunas, tabuleiro, s + 2);
            printf("Jogo gravado com sucesso!\n");
        } else if (isalpha(s[0]) && islower(s[0]) && isdigit(s[1])) {
            coordenada(linhas, colunas, tabuleiro, s);
        } else if (strcmp(s, "d") == 0) {
            if (desfazer(&topo, linhas, colunas, tabuleiro)==0) printf("Nenhuma ação para desfazer!\n");
            else printf("Desfeito!\n");
            printmatriz(linhas, colunas, tabuleiro);
        } else if (strcmp(s, "v") == 0) {
            cmd_v(linhas, colunas, tabuleiro);
        } else if (strcmp(s, "R") == 0) {
            cmd_resolver(&topo, linhas, colunas, tabuleiro);
            printf("Resolvido!\n");
            printmatriz(linhas, colunas, tabuleiro);
        } else if (s[0] == 'l' && s[1] == ' ' && (isalpha(s[2]) || isdigit(s[2]))) {
            sscanf(s + 2, "%s", nomeFicheiro);
            strcpy(nomeCompleto, "src/tabuleiros/");
            strcat(nomeCompleto, nomeFicheiro);
            int len = strlen(nomeFicheiro); 
            if (len < 4 || strcmp(&nomeFicheiro[len - 4], ".txt") != 0) {
                strcat(nomeCompleto, ".txt");
            }
            if (obter_dimensoes(nomeCompleto, &linhas, &colunas)) {
                printf("Erro ao abrir o ficheiro '%s'.\n", nomeCompleto);
            } else {
                static char matriz[100][100];
                tabuleiro = matriz;
                scanftab_e_stack(&topo, nomeCompleto, linhas, colunas, tabuleiro);
                printmatriz(linhas, colunas, tabuleiro);
                tabuleiroCarregado = 1;
            }
        } else if (strcmp(s, "a") == 0) {
            cmd_a(&topo, linhas, colunas, tabuleiro, 0);
            printf("Ajuda concluída.\n");
            printmatriz(linhas, colunas, tabuleiro);
        } else if (strcmp(s, "A") == 0) {
            cmd_a_grande(&topo, linhas, colunas, tabuleiro, 0);
            printf("Ajuda concluída.\n");
            printmatriz(linhas, colunas, tabuleiro);
        } else {
            printf("Comando inválido.\n");
        }
    }

    // Libertar memória
    Undo atual = topo;
    while (atual != NULL) {
        Undo proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    return 0;
}