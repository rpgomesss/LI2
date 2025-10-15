#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include "funcoes.h"

// Copia o “original” para tabuleiro_normalizado aplicando todos os Undos da stack.
// Ao mesmo tempo, adiciona na stack os nós inversos que “desfazer” usará depois.
int normalizar_tabuleiro(int linhas, int colunas,
                         char original[linhas][colunas],
                         Undo *topo,
                         char tabuleiro_normalizado[linhas][colunas])
{
    // 1) Começa com uma cópia exata de “original”
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro_normalizado[i][j] = original[i][j];
        }
    }
    
    int ind=0;
    // 2) Varre todos os Undos que já estavam na stack (do mais antigo ao mais recente)
    // 2.1) Contar quantos nós existem
    int count = 0;
    for (Undo node = *topo; node != NULL; node = node->prox) {
        count++;
    }
    if (count == 0) {
        // Nenhum Undo existente: tabuleiro_normalizado já é cópia de original.
        return 0;
    }

    // 2.2) Copiar ponteiros para array
    Undo *vetor = malloc(count * sizeof(Undo));
    int idx = 0;
    for (Undo node = *topo; node != NULL; node = node->prox) {
        vetor[idx++] = node;
    }
    // Agora vetor[0] = último nó inserido, vetor[count-1] = primeiro nó inserido.
    // Precisamos aplicar do nó mais antigo → mais recente, ou seja, de vetor[count-1] a vetor[0].

    for (int k = count - 1; k >= 0; k--) {
        Undo atual = vetor[k];
        int linha  = atual->linha;
        int coluna = atual->coluna;
        char valor_novo = atual->valor;

        // Antes de atualizar, salva o valor “atualmente presente” em tabuleiro_normalizado:
        char valor_antigo = tabuleiro_normalizado[linha][coluna];
        // Aplica o Undo para produzir o tabuleiro-normalizado:
        tabuleiro_normalizado[linha][coluna] = valor_novo;

        // Agora empurra na stack a “operação contrária”:
        // ou seja, quando chamarem desfazer de novo, queremos que (linha,coluna) volte a valor_antigo.
        push_undo(topo, linha, coluna, valor_antigo, ind++);
    }

    free(vetor);
    return ind;
}

void cmd_resolver(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    // Aloca espaço para um “board” auxiliar do mesmo tamanho
    char (*board)[colunas] = malloc(sizeof(char[linhas][colunas]));
    char s[20];
    int ind=normalizar_tabuleiro(linhas, colunas, tabuleiro, topo, board), temp;

    
    // 1) Testes nas primeiras e últimas linhas (j de 1 a colunas-2)
    for (int j = 1; j < colunas - 1; j++) {
        // i = 0 (primeira linha) e i = linhas-1 (última linha)
        for (int rep = 0; rep < 2; rep++) {
            int i = (rep == 0) ? 0 : (linhas - 1);

            // 1.1) Tripla adjacente: [j-1], [j], [j+1] iguais
            if (board[i][j] == board[i][j-1] && board[i][j] == board[i][j+1]) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + j, i + 1);
                if (cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
            // 1.2) Quadrado entre pares: [j-1] == [j+1]
            if (board[i][j-1] == board[i][j+1]) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + j, i + 1);
                if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
            // 1.3) Indução em pares horizontais
            // caso1: [j] == [j+1] e [j+2] == [j]  → marca em j+2
            if ( (j + 2 < colunas) &&
                 (board[i][j] == board[i][j+1]) &&
                 (board[i][j+2] == board[i][j]) ) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + (j + 2), i + 1);
                if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
            // caso2: [j] == [j-1] e [j-2] == [j]  → marca em j-2
            if ( (j - 2 >= 0) &&
                 (board[i][j] == board[i][j-1]) &&
                 (board[i][j-2] == board[i][j]) ) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + (j - 2), i + 1);
                if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
        }
    }

    
    // 2) Testes nas primeiras e últimas colunas (i de 1 a linhas-2)
    for (int i = 1; i < linhas - 1; i++) {
        // j = 0 (primeira coluna) e j = colunas-1 (última coluna)
        for (int rep = 0; rep < 2; rep++) {
            int j = (rep == 0) ? 0 : (colunas - 1);

            // 2.1) Tripla adjacente vertical: [i-1], [i], [i+1] iguais
            if (board[i][j] == board[i-1][j] && board[i][j] == board[i+1][j]) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + j, i + 1);
                if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
            // 2.2) Quadrado entre pares: [i-1] == [i+1]
            if (board[i-1][j] == board[i+1][j]) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + j, i + 1);
                if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
            // 2.3) Indução em pares verticais
            // caso1: [i] == [i+1] e [i+2] == [i]  → marca em i+2
            if ( (i + 2 < linhas) &&
                 (board[i][j] == board[i+1][j]) &&
                 (board[i+2][j] == board[i][j]) ) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + j, (i + 2) + 1);
                if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
            // caso2: [i] == [i-1] e [i-2] == [i]  → marca em i-2
            if ( (i - 2 >= 0) &&
                 (board[i][j] == board[i-1][j]) &&
                 (board[i-2][j] == board[i][j]) ) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + j, (i - 2) + 1);
                if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
        }
    }

    // 3) Testes em toda a “zona interna” (1 ≤ i ≤ linhas-2, 1 ≤ j ≤ colunas-2)
    for (int i = 1; i < linhas - 1; i++) {
        for (int j = 1; j < colunas - 1; j++) {
            // 3.1) Tripla adjacente (horizontal ou vertical)
            bool trip_horiz = (board[i][j] == board[i][j+1] && board[i][j] == board[i][j-1]);
            bool trip_vert  = (board[i][j] == board[i+1][j] && board[i][j] == board[i-1][j]);
            if (trip_horiz || trip_vert) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + j, i + 1);
                if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
            // 3.2) Quadrado entre pares (horizontal ou vertical)
            bool quad_horiz = (board[i][j-1] == board[i][j+1]);
            bool quad_vert  = (board[i-1][j] == board[i+1][j]);
            if (quad_horiz || quad_vert) {
                snprintf(s, sizeof(s), "b %c%d", 'a' + j, i + 1);
                if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
            }
        }
    }

    
    // 4) Chama cmd_a_grande antes do processo iterativo
    temp = cmd_a_grande(topo, linhas, colunas, board, ind);
    if (temp!=0) ind = temp++;

    
    // 5) Estrutura “floodfill + promoção de minúsculas + cmd_a_grande”
    //    num laço que repete enquanto o tabuleiro mudar
    char **anterior = malloc(linhas * sizeof(char *));
    for (int i = 0; i < linhas; i++) {
        anterior[i] = malloc(colunas * sizeof(char));
    }

    int alterado_flag;
    do {
        // 5.1) Copia o estado atual de 'board' para 'anterior'
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                anterior[i][j] = board[i][j];
            }
        }

        // 5.2) Processar todas as letras minúsculas em board
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (islower(board[i][j])) {
                    char letra_maiuscula = toupper(board[i][j]);
                    bool duplicado = false;
                    bool ambiguo   = false;

                    // Varre toda a coluna j
                    for (int x = 0; x < linhas; x++) {
                        if (x != i && board[x][j] == letra_maiuscula) duplicado = true;
                        if (x != i && board[x][j] == board[i][j])   ambiguo   = true;
                    }
                    // Varre toda a linha i
                    for (int y = 0; y < colunas; y++) {
                        if (y != j && board[i][y] == letra_maiuscula) duplicado = true;
                        if (y != j && board[i][y] == board[i][j])     ambiguo   = true;
                    }

                    snprintf(s, sizeof(s), "%c %c%d", (duplicado ? 'a' : 'b'), 'a' + j, i + 1);
                    if (duplicado) {
                        if(cmd_r(topo, linhas, colunas, board, s, ind)) ind++;
                    }
                    else if (!ambiguo) {
                        if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
                    }
                }
            }
        }

        // 5.3) Reaplica cmd_a_grande após os comandos de r/b
        temp = cmd_a_grande(topo, linhas, colunas, board, ind);
        if (temp!=0) ind = temp++;

        // 5.4) Identificar um componente “branco” (maiúscula) e fazer floodfill
        int visitado[linhas][colunas];
        memset(visitado, 0, sizeof(visitado));

        // Encontra o primeiro maiúsculo para iniciar floodfill usando loops controlados por flag
        bool achou_ponto = false;
        int ii = 0;
        while (ii < linhas && achou_ponto == false) {
            int jj = 0;
            while (jj < colunas && achou_ponto == false) {
                if (isupper(board[ii][jj])) {
                    floodfill(ii, jj, linhas, colunas, board, visitado);
                    achou_ponto = true;
                }
                jj++;
            }
            ii++;
        }

        // 5.5) Para cada maiúscula não visitada, verificar vizinhança e promover minúscula
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (isupper(board[i][j]) && !visitado[i][j]) {
                    int di[4] = { -1, +1,  0,  0 };
                    int dj[4] = {  0,  0, -1, +1 };
                    int count_minus = 0;
                    int li = -1, lj = -1;
                    bool valido = true;

                    // Varre cada um dos quatro vizinhos
                    for (int d = 0; d < 4; d++) {
                        int ni = i + di[d];
                        int nj = j + dj[d];

                        // Se estiver fora do tabuleiro, considera como "parede"
                        if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
                            char c = board[ni][nj];
                            if (islower(c)) {
                                count_minus++;
                                li = ni;
                                lj = nj;
                            }
                            else if (c != '#') {
                                // Se vizinho não é '#' nem minúscula, não é isolado só por #+1 minúscula
                                valido = false;
                            }
                        }
                        // caso “fora do tabuleiro”: tratamos implicitamente como parede, sem ação extra
                    }

                    // Se estava “isolada” (apenas # e UMA minúscula), promovemos essa minúscula
                    if (valido && count_minus == 1) {
                        snprintf(s, sizeof(s), "b %c%d", 'a' + lj, li + 1);
                        if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
                    }
                }
            }
        }

        // 5.6) Reaplica cmd_a_grande após promoção de minúsculas
        temp = cmd_a_grande(topo, linhas, colunas, board, ind);
        if (temp!=0) ind = temp++;

        // 5.7) Verifica se algo mudou (board ≠ anterior) → controla repetição
        alterado_flag = 0;
        int iii = 0;
        while (iii < linhas && alterado_flag == 0) {
            int jjj = 0;
            while (jjj < colunas && alterado_flag == 0) {
                if (board[iii][jjj] != anterior[iii][jjj]) {
                    alterado_flag = 1;
                }
                jjj++;
            }
            iii++;
        }

    } while (alterado_flag);

    
    // 6) Última verificação de casas brancas não ligadas e marcações de 'caminho'
    int visitado[linhas][colunas];
    memset(visitado, 0, sizeof(visitado));

    // Encontrar um componente branco (maiúscula) para iniciar floodfill
    bool achou_componente = false;
    int aaa = 0;
    while (aaa < linhas && achou_componente == false) {
        int bbb = 0;
        while (bbb < colunas && achou_componente == false) {
            if (isupper(board[aaa][bbb])) {
                floodfill(aaa, bbb, linhas, colunas, board, visitado);
                achou_componente = true;
            }
            bbb++;
        }
        aaa++;
    }

    int caminho[linhas][colunas];
    memset(caminho, 0, sizeof(caminho));

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (isupper(board[i][j]) && !visitado[i][j]) {
                caminho[i][j] = 1;
            }
        }
    }

    // 6.1) Para cada branca isolada, tentar promover minúscula vizinha
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (isupper(board[i][j]) && !visitado[i][j]) {
                int di[4] = { -1, +1,  0,  0 };
                int dj[4] = {  0,  0, -1, +1 };
                int count_minus = 0;
                int li = -1, lj = -1;
                bool valido = true;

                for (int d = 0; d < 4; d++) {
                    int ni = i + di[d];
                    int nj = j + dj[d];

                    if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
                        char c = board[ni][nj];
                        if (islower(c)) {
                            count_minus++;
                            li = ni;
                            lj = nj;
                        }
                        else if (c == '#' || caminho[ni][nj] == 1) {
                            // parede ou outro branco já marcado → tudo bem
                        }
                        else {
                            valido = false;
                        }
                    }
                    // fora do tabuleiro → considerado “parede”
                }

                if (valido && count_minus == 1) {
                    snprintf(s, sizeof(s), "b %c%d", 'a' + lj, li + 1);
                    if(cmd_b(topo, linhas, colunas, board, s, ind)) ind++;
                }
            }
        }
    }

    temp = cmd_a_grande(topo, linhas, colunas, board, ind);
    if (temp!=0) ind = temp++;

    
    // 7) Se ainda houver minúsculas restantes, chamar resolve_aux
    int precisa_resolver = 0;
    int ccc = 0;
    while (ccc < linhas && precisa_resolver == 0) {
        int ddd = 0;
        while (ddd < colunas && precisa_resolver == 0) {
            if (board[ccc][ddd] > 'a' && board[ccc][ddd] < 'z') {
                precisa_resolver = 1;
            }
            ddd++;
        }
        ccc++;
    }
    if (precisa_resolver) {
        desfazer(topo, linhas, colunas, board);
        resolve_aux(topo, linhas, colunas, board);
    }


    // 8) Copia do 'board' final de volta para 'tabuleiro' original
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = board[i][j];
        }
    }

    // Liberta memória auxiliar
    for (int i = 0; i < linhas; i++) {
        free(anterior[i]);
    }
    free(anterior);
    free(board);
}
