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

void test_cmd_r(void) {
    Undo topo = NULL;
    char tabuleiro[2][2] = {{'a', 'b'}, {'c', 'd'}};

    char comando1[] = "r a2";
    cmd_r(&topo, 2, 2, tabuleiro, comando1, 0);
    CU_ASSERT_EQUAL(tabuleiro[1][0], '#');
    CU_ASSERT_PTR_NOT_NULL(topo);
    CU_ASSERT_EQUAL(topo->linha, 1);
    CU_ASSERT_EQUAL(topo->coluna, 0);
    CU_ASSERT_EQUAL(topo->valor, 'c');

    free_undo(topo);
}

void test_cmd_r_fail(void) {
    Undo topo = NULL;
    char tabuleiro[2][2] = {{'a', 'b'}, {'c', 'd'}};

    // Caso 1: coordenada inválida (fora dos limites)
    char comando1[] = "r z9";
    cmd_r(&topo, 2, 2, tabuleiro, comando1, 0);
    if (topo != NULL) {
        CU_FAIL("Stack de Undo não deveria ser alterada com coordenada inválida.");
        free_undo(topo);
        topo = NULL;
    }

    // Caso 2: célula já marcada com '#', não deve fazer nada
    tabuleiro[0][1] = '#';
    char comando2[] = "r b1";  // b1 é [0][1], que já está '#'
    cmd_r(&topo, 2, 2, tabuleiro, comando2, 0);
    if (topo != NULL) {
        CU_FAIL("Stack de Undo não deveria ser alterada ao tentar restaurar célula já marcada.");
        free_undo(topo);
        topo = NULL;
    }

    // Confirmar que o tabuleiro permanece o mesmo após ambos os comandos inválidos
    if (tabuleiro[0][0] != 'a' || tabuleiro[0][1] != '#' ||
        tabuleiro[1][0] != 'c' || tabuleiro[1][1] != 'd') {
        CU_FAIL("Tabuleiro não deveria ser modificado por comandos inválidos.");
    }
}