#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <stdlib.h>
#include <dirent.h>
#include "funcoes.h"

// Função que imprime o conteúdo de um tabuleiro (matriz de caracteres) no terminal
void printmatriz(int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    // Imprime o título antes da matriz
    printf("Tabuleiro:\n");

    // Percorre todas as linhas do tabuleiro
    for (int i = 0; i < linhas; i++) {
        // Percorre todas as colunas da linha atual
        for (int j = 0; j < colunas; j++) {
            // Imprime o caractere correspondente seguido de um espaço
            printf("%c ", tabuleiro[i][j]);
        }
        // Após imprimir uma linha completa, quebra a linha para passar à próxima
        printf("\n");
    }
}

// Função auxiliar que adiciona a ultima jogada ao topo da stack de jogadas 
void push_undo(Undo *topo, int linha, int coluna, char valor, int i) {
    // Aloca memória para um novo elemento da pilha Undo
    Undo novo = malloc(sizeof(struct Undo));
    if (novo == NULL) {
        printf("Erro de alocação!\n");
        return;
    }

    // Atribui os valores da jogada ao novo elemento
    novo->linha = linha;
    novo->coluna = coluna;
    novo->valor = valor;
    novo->ind = i; 

    // Liga o novo elemento ao topo atual da pilha
    novo->prox = *topo;

    // Atualiza o topo para apontar para o novo elemento
    *topo = novo;
}

// Função auxiliar que desfaz as ações da stack até atingir o índice desejado
void desfazer_acoes(Undo *topo, int qtd, int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    int i = 0; 

    // Continua enquanto houver ações a desfazer e a pilha não estiver vazia
    while (i < qtd && *topo != NULL) {
        // Obtém os dados da ação mais recente no topo da stack
        int linha = (*topo)->linha;
        int coluna = (*topo)->coluna;
        char valor = (*topo)->valor;

        // Restaura o valor no tabuleiro na posição correspondente
        tabuleiro[linha][coluna] = valor;

        // Remove o elemento do topo da stack
        Undo tmp = *topo;        
        *topo = (*topo)->prox;   
        free(tmp);               

        i++; 
    }
}

// Função que desfaz a última ação (ou grupo de ações) da pilha 
int desfazer(Undo *topo, int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    // Verifica se a stack de Undo está vazia
    if (*topo == NULL)
        return 0; // Não há nada para desfazer, sai da função

    // Calcula a quantidade de ações consecutivas a desfazer com base no índice
    int qtd = (*topo)->ind + 1;

    // Chama a função auxiliar que desfaz as ações e atualiza o tabuleiro
    desfazer_acoes(topo, qtd, linhas, colunas, tabuleiro);
    return 1;
}

// Lê e preenche o tabuleiro a partir do ficheiro
int ler_tabuleiro(FILE *fptr, int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    // Cria um buffer temporário para armazenar cada linha lida do ficheiro.
    // O tamanho é colunas + 2 para garantir espaço para o '\n' e o '\0'.
    char linha[colunas + 2];

    // Percorre todas as linhas esperadas do tabuleiro
    for (int i = 0; i < linhas; i++) {
        // Lê uma linha completa do ficheiro com fgets
        if (fgets(linha, sizeof(linha), fptr) == NULL) {
            printf("Erro ao ler a linha %d do tabuleiro.\n", i + 1);
            return 1; // Código de erro
        }

        // Copia os caracteres lidos para a matriz do tabuleiro
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = linha[j];
        }
    }
    return 0;
}

// Lê a stack de undos do ficheiro e carrega na memória
int ler_stack_undo(FILE *fptr, Undo *topo) {
    int n_undos = 0;

    // Tenta ler o número total de jogadas na pilha a partir do ficheiro
    if (fscanf(fptr, "%d", &n_undos) != 1) {
        // Se falhar, indica que não há stack no ficheiro
        printf("Nenhuma stack de Undo encontrada no ficheiro.\n");
        return 1;
    }

    // Percorre cada jogada armazenada no ficheiro
    for (int i = 0; i < n_undos; i++) {
        int linhaUndo, colunaUndo, indUndo;
        char valorUndo;

        // Lê a linha, coluna e o valor da jogada
        if (fscanf(fptr, "%d %d %c %d", &linhaUndo, &colunaUndo, &valorUndo, &indUndo) != 4) {
            // Se falhar a leitura, informa o erro e sai
            printf("Erro ao ler ação de Undo %d.\n", i + 1);
            return 2;
        }

        // Adiciona a jogada lida à pilha 
        push_undo(topo, linhaUndo, colunaUndo, valorUndo, indUndo);
    }

    // Informa o sucesso e o número de jogadas carregadas
    printf("Stack de Undo carregada com %d movimentos.\n", n_undos);
    return 0; 
}

// Função principal que carrega tabuleiro e stack
int scanftab_e_stack(Undo *topo, char *nomeFicheiro, int linhas, int colunas, char tabuleiro[linhas][colunas]) {
    
    // Tenta abrir o ficheiro para leitura
    FILE *fptr = fopen(nomeFicheiro, "r");
    if (fptr == NULL) {
        // Erro ao abrir o ficheiro
        printf("Erro ao abrir o ficheiro %s.\n", nomeFicheiro);
        return 1;
    }

    // Lê as dimensões do ficheiro (mas ignora o conteúdo real, pois já temos as dimensões esperadas)
    int linhasIgnorar, colunasIgnorar;
    if (fscanf(fptr, "%d %d\n", &linhasIgnorar, &colunasIgnorar) != 2) {
        // Se a leitura falhar, encerra o ficheiro e retorna erro
        printf("Erro ao ler dimensões do ficheiro.\n");
        fclose(fptr);
        return 2;
    }

    // Lê o tabuleiro linha por linha a partir do ficheiro
    if (ler_tabuleiro(fptr, linhas, colunas, tabuleiro) != 0) {
        // Se a leitura do tabuleiro falhar, fecha o ficheiro e retorna erro
        fclose(fptr);
        return 3;
    }

    // Lê a stack de undo a partir do ficheiro
    int erro_stack = ler_stack_undo(fptr, topo);

    // Fecha o ficheiro após a leitura
    fclose(fptr);

    // Retorna o resultado da leitura da stack (0 se sucesso, outro valor se erro)
    return erro_stack;
}

// Função que abre um ficheiro e lê o número de linhas e colunas de um tabuleiro.
int obter_dimensoes(const char *nomeFicheiro, int *linhas, int *colunas) {
    // Tenta abrir o ficheiro em modo de leitura
    FILE *fptr = fopen(nomeFicheiro, "r");

    // Verifica se o ficheiro foi aberto corretamente
    if (fptr == NULL) {
        return 1; 
    }

    // Lê dois inteiros do ficheiro, correspondentes às dimensões
    if (fscanf(fptr, "%d %d", linhas, colunas) != 2) {
        // Se a leitura falhar, mostra uma mensagem de erro
        printf("Erro ao ler as dimensões do tabuleiro.\n");
        fclose(fptr);
        return 2; 
    }

    // Fecha o ficheiro após a leitura bem-sucedida
    fclose(fptr);
    return 0;
}
