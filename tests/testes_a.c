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

void teste_cmd_a(void) {
    // Teste 1: Verificar se a função cmd_a risca corretamente as letras duplicadas
    char tabuleiro[3][3] = {
        {'A', 'B', 'C'},
        {'a', 'A', 'D'},
        {'B', 'C', 'E'}
    };
    Undo topo = NULL;
    int push_id = 0;

    push_id = cmd_a(&topo, 3, 3, tabuleiro, push_id);

    CU_ASSERT(tabuleiro[0][0] == 'A');
    CU_ASSERT(tabuleiro[1][0] == '#');
    CU_ASSERT(tabuleiro[2][0] == 'B');
    CU_ASSERT(tabuleiro[0][1] == 'B');
    CU_ASSERT(tabuleiro[1][2] == 'D');

    free_undo(topo);

    // Teste 2: Verificar quando não há duplicados
    char tabuleiro_sem_duplicados[3][3] = {
        {'X', 'Y', 'Z'},
        {'A', 'B', 'C'},
        {'D', 'E', 'F'}
    };
    Undo topo2 = NULL;
    char copia[3][3];
    memcpy(copia, tabuleiro_sem_duplicados, sizeof(tabuleiro_sem_duplicados));

    push_id = cmd_a(&topo2, 3, 3, tabuleiro_sem_duplicados, push_id);

    CU_ASSERT(memcmp(copia, tabuleiro_sem_duplicados, sizeof(tabuleiro_sem_duplicados)) == 0);
    free_undo(topo2);

    // Teste 3: Verificar quando não há duplicados a função não altera o tabuleiro
    char tabuleiro_sem_modificacao[3][3] = {
        {'X', 'Y', 'Z'},
        {'A', 'B', 'C'},
        {'D', 'E', 'F'}
    };
    Undo topo3 = NULL;
    char copia2[3][3];
    memcpy(copia2, tabuleiro_sem_modificacao, sizeof(tabuleiro_sem_modificacao));

    push_id = cmd_a(&topo3, 3, 3, tabuleiro_sem_modificacao, push_id);

    CU_ASSERT(memcmp(copia2, tabuleiro_sem_modificacao, sizeof(tabuleiro_sem_modificacao)) == 0);
    free_undo(topo3);

    // Teste 4: verificar_riscadas agora é feita dentro de cmd_a
    char tabuleiro_bordas[3][3] = {
        {'A', '#', 'B'},
        {'#', 'C', '#'},
        {'D', 'E', 'F'}
    };
    Undo topo4 = NULL;
    push_id = cmd_a(&topo4, 3, 3, tabuleiro_bordas, push_id);

    CU_ASSERT(tabuleiro_bordas[0][1] == '#');
    CU_ASSERT(tabuleiro_bordas[1][0] == '#');
    CU_ASSERT(tabuleiro_bordas[1][1] == 'C');
    CU_ASSERT(tabuleiro_bordas[1][2] == '#');
    free_undo(topo4);

    // Teste 5: Riscagem de letras minúsculas na mesma linha de uma maiúscula correspondente
    char tabuleiro_linha[3][3] = {
        {'A', 'a', 'C'},
        {'D', 'E', 'F'},
        {'G', 'H', 'I'}
    };
    Undo topo5 = NULL;
    push_id = cmd_a(&topo5, 3, 3, tabuleiro_linha, push_id);

    CU_ASSERT(tabuleiro_linha[0][1] == '#');
    CU_ASSERT(tabuleiro_linha[0][0] == 'A');
    CU_ASSERT(tabuleiro_linha[0][2] == 'C');
    free_undo(topo5);
}

void test_verifica_riscadas_converte_minuscula(void) {
    int linhas = 3, colunas = 3;
    char tab[3][3] = {
        {'a', '#', 'c'},
        {'d', 'e', 'f'},
        {'g', 'h', 'i'}
    };
    Undo pilha = NULL;
    int push_id = 0;

    verifica_riscadas(linhas, colunas, tab, &pilha, &push_id);

    // Verifica se a célula (0,0) foi convertida de 'a' para 'A'
    CU_ASSERT(tab[0][0] == 'A'); // esquerda de '#'
    // Verifica se a célula (0,2) foi convertida de 'c' para 'C'
    CU_ASSERT(tab[0][2] == 'C'); // direita de '#'

    // Verifica se push_id aumentou 2 vezes
    assert(push_id == 3);

    // Liberta memória da pilha
    while (pilha != NULL) {
        Undo tmp = pilha;
        pilha = pilha->prox;
        free(tmp);
    }
}


void teste_cmd_a_grande(void) {
    // Teste 1: Verificar propagação total até o tabuleiro estabilizar
    char tabuleiro1[3][3] = {
        {'A', 'a', 'C'},
        {'c', 'A', 'D'},
        {'d', 'e', 'F'}
    };

    char esperado1[3][3] = {
        {'A', '#', 'C'},
        {'c', 'A', 'D'},
        {'d', 'e', 'F'}  // 'd' e 'e' não são afetadas
    };

    Undo topo1 = NULL;
    cmd_a_grande(&topo1, 3, 3, tabuleiro1, 0);

    CU_ASSERT(memcmp(tabuleiro1, esperado1, sizeof(tabuleiro1)) == 0);
    free_undo(topo1);

    // Teste 2: Verificar que tabuleiro estável não muda
    char tabuleiro2[2][2] = {
        {'X', 'Y'},
        {'Z', 'W'}
    };
    char copia2[2][2];
    memcpy(copia2, tabuleiro2, sizeof(tabuleiro2));
    Undo topo2 = NULL;

    cmd_a_grande(&topo2, 2, 2, tabuleiro2, 0);
    CU_ASSERT(memcmp(copia2, tabuleiro2, sizeof(tabuleiro2)) == 0);
    free_undo(topo2);

    // Teste 3: Teste com risco apenas em colunas
    char tabuleiro3[3][3] = {
        {'A', 'B', 'C'},
        {'a', 'E', 'F'},
        {'b', 'G', 'H'}
    };
    
    char esperado3[3][3] = {
        {'A', 'B', 'C'},
        {'#', 'E', 'F'},  // 'a' riscado por 'A' na mesma coluna
        {'B', 'G', 'H'}   // 'b' NÃO riscado — sem 'B' na mesma coluna
    };
    
    Undo topo3 = NULL;
    cmd_a_grande(&topo3, 3, 3, tabuleiro3, 0);
    
    // Verificação correta:
    CU_ASSERT(memcmp(tabuleiro3, esperado3, sizeof(tabuleiro3)) == 0);
    
    free_undo(topo3);
    

    // Teste 4: Múltiplas iterações de risco
    char tabuleiro4[3][3] = {
        {'A', 'b', 'C'},
        {'a', 'B', 'd'},
        {'e', 'f', 'g'}
    };
    Undo topo4 = NULL;
    cmd_a_grande(&topo4, 3, 3, tabuleiro4, 0);

    // A 'a' e 'b' devem ser riscadas devido às maiúsculas 'A' e 'B'
    CU_ASSERT(tabuleiro4[1][0] == '#');
    CU_ASSERT(tabuleiro4[0][1] == '#');
    CU_ASSERT(tabuleiro4[1][1] == 'B');
    free_undo(topo4);
}
