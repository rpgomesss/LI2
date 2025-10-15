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

void test_cmd_b(void) {
    Undo topo = NULL;
    char tabuleiro[2][2] = {{'a', 'b'}, {'c', 'd'}};

    char comando1[] = "b a1";
    cmd_b(&topo, 2, 2, tabuleiro, comando1, 0);
    CU_ASSERT_EQUAL(tabuleiro[0][0], 'A');
    CU_ASSERT_PTR_NOT_NULL(topo);
    CU_ASSERT_EQUAL(topo->linha, 0);
    CU_ASSERT_EQUAL(topo->coluna, 0);
    CU_ASSERT_EQUAL(topo->valor, 'a');

    free_undo(topo);
}

void test_cmd_b_fail(void) {
    Undo topo = NULL;
    char tabuleiro[2][2] = {{'a', 'b'}, {'c', 'd'}};
    char comando1[] = "b c1"; // comando inválido (fora dos limites)

    // Tenta executar comando inválido
    cmd_b(&topo, 2, 2, tabuleiro, comando1, 0);

    // Verifica se o tabuleiro se manteve inalterado
    if (tabuleiro[0][0] != 'a' || tabuleiro[0][1] != 'b' ||
        tabuleiro[1][0] != 'c' || tabuleiro[1][1] != 'd') {
        CU_FAIL("Tabuleiro não deveria ser modificado devido a coordenadas inválidas.");
    }

    // Verifica que a stack está vazia
    if (topo != NULL) {
        CU_FAIL("Stack de Undo não deveria ser alterada com comando inválido.");
        free_undo(topo);
        topo = NULL;
    }

    // Comando válido: pinta a célula a1
    char comando2[] = "b a1";
    cmd_b(&topo, 2, 2, tabuleiro, comando2, 0);
    free_undo(topo);
    topo = NULL;

    // Tenta pintar novamente a célula já em branco
    cmd_b(&topo, 2, 2, tabuleiro, comando2, 0);
    if (topo != NULL) {
        CU_FAIL("Stack de Undo deveria estar vazia para célula já em branco.");
        free_undo(topo);
        topo = NULL;
    }
}