#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <assert.h>
#include "../src/funcoes.h"
#include "testes.h"

void test_gravar_jogo(void) {
    Undo topo = NULL;
    char tabuleiro[2][2] = {{'a', 'b'}, {'c', 'd'}};
    char filename[1000] = "jogoteste1";

    // Adicionar ações à pilha de undo
    push_undo(&topo, 0, 0, 'z', 0); // Base
    push_undo(&topo, 1, 1, 'y', 1); // Topo

    // Grava o jogo
    gravarjogo(&topo, 2, 2, tabuleiro, filename);

    // Verifica o ficheiro gravado
    strcpy(filename, "src/tabuleiros/jogoteste1.txt");
    FILE *f = fopen(filename, "r");
    CU_ASSERT_PTR_NOT_NULL(f);

    // Verifica dimensões
    int linhas, colunas;
    CU_ASSERT_EQUAL(fscanf(f, "%d %d", &linhas, &colunas), 2);
    CU_ASSERT_EQUAL(linhas, 2);
    CU_ASSERT_EQUAL(colunas, 2);

    // Verifica tabuleiro
    char linha1[10], linha2[10];
    CU_ASSERT_EQUAL(fscanf(f, "%s", linha1), 1);
    CU_ASSERT_EQUAL(fscanf(f, "%s", linha2), 1);
    CU_ASSERT_STRING_EQUAL(linha1, "ab");
    CU_ASSERT_STRING_EQUAL(linha2, "cd");

    // Verifica número de ações de undo
    int contador;
    CU_ASSERT_EQUAL(fscanf(f, "%d", &contador), 1);
    CU_ASSERT_EQUAL(contador, 2);

    // Lê e verifica as duas ações
    int linha, coluna, indicador;
    char valor;

    CU_ASSERT_EQUAL(fscanf(f, "%d %d %c %d", &linha, &coluna, &valor, &indicador), 4);
    CU_ASSERT_EQUAL(linha, 1);
    CU_ASSERT_EQUAL(coluna, 1);
    CU_ASSERT_EQUAL(valor, 'y');
    CU_ASSERT_EQUAL(indicador, 1);

    CU_ASSERT_EQUAL(fscanf(f, "%d %d %c %d", &linha, &coluna, &valor, &indicador), 4);
    CU_ASSERT_EQUAL(linha, 0);
    CU_ASSERT_EQUAL(coluna, 0);
    CU_ASSERT_EQUAL(valor, 'z');
    CU_ASSERT_EQUAL(indicador, 0);

    fclose(f);
    remove(filename);

    // Libertar a memória
    free_undo(topo);
}


void test_gravar_jogo_fail(void) {
    Undo *topo = NULL;
    char tabuleiro[2][2] = {{'a', 'b'}, {'c', 'd'}};
    char nomeInvalido[] = "/caminho/inexistente/jogoteste_invalido";

    // A função deve falhar ao tentar abrir o ficheiro para escrita
    gravarjogo(topo, 2, 2, tabuleiro, nomeInvalido);

    // Verificamos se o ficheiro NÃO foi criado
    FILE *f = fopen(nomeInvalido, "r");
    if (f != NULL) {
        fclose(f);
        CU_FAIL("Ficheiro não deveria ser criado com caminho inválido.");
        remove(nomeInvalido);
    }
}

void test_nome_curto_sem_extensao(void) {
    Undo topo = NULL;
    char tabuleiro[2][2] = {{'a', 'b'}, {'c', 'd'}};
    char filename[] = "xy";  // Apenas 2 letras, menor que 4

    gravarjogo(&topo, 2, 2, tabuleiro, filename);

    FILE *f = fopen("src/tabuleiros/xy.txt", "r");
    CU_ASSERT_PTR_NOT_NULL(f);
    if (f) fclose(f);
    remove("src/tabuleiros/xy.txt");

    free_undo(topo);
}

void test_nome_com_extensao_errada(void) {
    Undo topo = NULL;
    char tabuleiro[2][2] = {{'a', 'b'}, {'c', 'd'}};
    char filename[] = "dados.csv";  // Termina em .csv, não .txt

    gravarjogo(&topo, 2, 2, tabuleiro, filename);

    FILE *f = fopen("src/tabuleiros/dados.csv.txt", "r");  // A função adiciona ".txt"
    CU_ASSERT_PTR_NOT_NULL(f);
    if (f) fclose(f);
    remove("src/tabuleiros/dados.csv.txt");

    free_undo(topo);
}