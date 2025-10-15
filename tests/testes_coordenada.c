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

void test_coordenada(void) {
    char tabuleiro[2][2] = { {'A', 'B'}, {'C', 'D'} };

    // Coordenada válida: c = 'a' (coluna 0), l = 1 (linha 0)
    int r1 = coordenada(2, 2, tabuleiro, "x a1");
    CU_ASSERT_EQUAL(r1, 0);

    // Coordenada válida: c = 'b' (coluna 1), l = 2 (linha 1)
    int r2 = coordenada(2, 2, tabuleiro, "x b2");
    CU_ASSERT_EQUAL(r2, 0);
}

void test_coordenada_fail(void) {
    char tabuleiro[2][2] = { {'A', 'B'}, {'C', 'D'} };

    // Coluna inválida
    if (coordenada(2, 2, tabuleiro, "x c1") == 0) {
        CU_FAIL("Coordenada inválida 'c1' foi aceite.");
    }

    // Linha inválida
    if (coordenada(2, 2, tabuleiro, "x a3") == 0) {
        CU_FAIL("Coordenada inválida 'a3' foi aceite.");
    }

    // String curta (causava overflow antes da verificação de tamanho)
    if (coordenada(2, 2, tabuleiro, "x ") == 0) {
        CU_FAIL("Coordenada vazia ou mal formatada foi aceite.");
    }

    // Totalmente fora dos limites
    if (coordenada(2, 2, tabuleiro, "x z9") == 0) {
        CU_FAIL("Coordenada completamente fora dos limites foi aceite.");
    }
}