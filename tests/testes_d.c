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

void test_push_and_desfazer(void) {
    Undo topo = NULL;
    char tabuleiro[2][2] = { {'a', 'b'}, {'c', 'd'} };

    // Teste 1: Desfazer uma única alteração
    push_undo(&topo, 0, 0, 'z', 0);  
    CU_ASSERT_PTR_NOT_NULL(topo);
    CU_ASSERT_EQUAL(topo->linha, 0);
    CU_ASSERT_EQUAL(topo->coluna, 0);
    CU_ASSERT_EQUAL(topo->valor, 'z');
    CU_ASSERT_EQUAL(topo->ind, 0);
    CU_ASSERT_PTR_NULL(topo->prox);

    tabuleiro[0][0] = 'x';
    desfazer(&topo, 2, 2, tabuleiro);
    CU_ASSERT_EQUAL(tabuleiro[0][0], 'z');
    CU_ASSERT_PTR_NULL(topo);

    // Teste 2: Desfazer múltiplas alterações agrupadas (ind = 2 → 3 ações)
    push_undo(&topo, 1, 1, 'y', 0);  // ind = 0
    push_undo(&topo, 0, 1, 'w', 0);  // ind = 0
    push_undo(&topo, 0, 0, 'v', 2);  // ind = 2 → desfaz 3 ações

    // Verificações após empilhar alterações
    CU_ASSERT_PTR_NOT_NULL(topo);
    CU_ASSERT_EQUAL(topo->linha, 0);
    CU_ASSERT_EQUAL(topo->coluna, 0);
    CU_ASSERT_EQUAL(topo->valor, 'v');
    CU_ASSERT_EQUAL(topo->ind, 2);

    // Modificações no tabuleiro
    tabuleiro[0][0] = 'm';
    tabuleiro[0][1] = 'n';
    tabuleiro[1][1] = 'o';

    // Desfazer as três alterações agrupadas
    desfazer(&topo, 2, 2, tabuleiro);
    CU_ASSERT_EQUAL(tabuleiro[0][0], 'v');
    CU_ASSERT_EQUAL(tabuleiro[0][1], 'w');
    CU_ASSERT_EQUAL(tabuleiro[1][1], 'y');
    CU_ASSERT_PTR_NULL(topo);
}

void test_push_and_desfazer_fail(void) {
    Undo topo = NULL;
    char tabuleiro[2][2] = { {'a', 'b'}, {'c', 'd'} };

    // 1. Tentar desfazer com stack vazia
    desfazer(&topo, 2, 2, tabuleiro);

    // Stack deve continuar nula e tabuleiro inalterado
    if (topo != NULL) {
        CU_FAIL("Stack de Undo deveria estar vazia.");
        free_undo(topo);
        topo = NULL;
    }

    if (tabuleiro[0][0] != 'a' || tabuleiro[0][1] != 'b' ||
        tabuleiro[1][0] != 'c' || tabuleiro[1][1] != 'd') {
        CU_FAIL("Tabuleiro não deveria ser alterado ao desfazer sem ações.");
    }

    

    // 2. Usar desfazer depois de esvaziar a stack
    push_undo(&topo, 0, 0, 'x',1);
    tabuleiro[0][0] = 'z';
    desfazer(&topo, 2, 2, tabuleiro); // desfaz com sucesso
    desfazer(&topo, 2, 2, tabuleiro); // deve apenas printar mensagem

    if (topo != NULL) {
        CU_FAIL("Stack deveria estar vazia após duas chamadas de desfazer.");
        free_undo(topo);
    }

    if (tabuleiro[0][0] != 'x') {
        CU_FAIL("Valor da célula deveria ser restaurado corretamente.");
    }
}