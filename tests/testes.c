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

void free_undo(Undo topo) {
    while (topo != NULL) {
        Undo temp = topo;
        topo = topo->prox;
        free(temp);
    }
}

void test_printmatriz(void) {
    char tabuleiro[2][2] = {{'a', 'b'}, {'c', 'd'}};
    printmatriz(2, 2, tabuleiro);
}

void test_obter_dimensoes(void) {
    int linhas = 0, colunas = 0;
    int r;

    FILE *f = fopen("src/tabuleiros/dimensao.txt", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "3 3\n");
    fclose(f);

    r = obter_dimensoes("src/tabuleiros/dimensao.txt", &linhas, &colunas);
    CU_ASSERT_EQUAL(r, 0);
    CU_ASSERT_EQUAL(linhas, 3);
    CU_ASSERT_EQUAL(colunas, 3);

    remove("src/tabuleiros/dimensao.txt");
}

void test_obter_dimensoes_fail(void) {
    int linhas = 0, colunas = 0;
    int r;

    // Caso 1: ficheiro inexistente
    r = obter_dimensoes("src/tabuleiros/ficheiro_inexistente.txt", &linhas, &colunas);
    if (r != 1) {
        CU_FAIL("Esperava erro ao abrir ficheiro inexistente (código 1).");
    }

    // Caso 2: ficheiro mal formatado (apenas 1 número em vez de 2)
    FILE *f = fopen("src/tabuleiros/dimensao_malformatado.txt", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "5\n");
    fclose(f);

    r = obter_dimensoes("src/tabuleiros/dimensao_malformatado.txt", &linhas, &colunas);
    if (r != 2) {
        CU_FAIL("Esperava erro de leitura das dimensões mal formatadas (código 2).");
    }

    remove("src/tabuleiros/dimensao_malformatado.txt");
}

void test_scanftab_e_stack(void) {
    Undo topo = NULL;
    int linhas = 2, colunas = 2;
    char tabuleiro[2][2];

    FILE *f = fopen("src/tabuleiros/tabuleiro_valido.txt", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "2 2\nab\ncd\n1\n0 1 x 0\n");
    fclose(f);

    int res = scanftab_e_stack(&topo, "src/tabuleiros/tabuleiro_valido.txt", linhas, colunas, tabuleiro);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(tabuleiro[0][0], 'a');
    CU_ASSERT_EQUAL(tabuleiro[0][1], 'b');
    CU_ASSERT_EQUAL(tabuleiro[1][0], 'c');
    CU_ASSERT_EQUAL(tabuleiro[1][1], 'd');

    CU_ASSERT_PTR_NOT_NULL(topo);
    if (topo != NULL) {
        CU_ASSERT_EQUAL(topo->linha, 0);
        CU_ASSERT_EQUAL(topo->coluna, 1);
        CU_ASSERT_EQUAL(topo->valor, 'x');
        CU_ASSERT_EQUAL(topo->ind, 0);  // assumindo que existe este campo
        CU_ASSERT_PTR_NULL(topo->prox);
    }

    free_undo(topo);
    remove("src/tabuleiros/tabuleiro_valido.txt");
}


void test_scanftab_e_stack_fail(void) {
    Undo topo = NULL;
    int linhas = 2, colunas = 2;
    char tabuleiro[2][2];
    int res;

    // Caso 1: Ficheiro inexistente
    res = scanftab_e_stack(&topo, "src/tabuleiros/nao_existe.txt", linhas, colunas, tabuleiro);
    if (res != 1) CU_FAIL("Esperava erro ao abrir ficheiro inexistente (código 1)");

    // Caso 2: Dimensões mal formatadas
    FILE *f = fopen("src/tabuleiros/tabuleiro_dim_invalidas.txt", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "2\n"); // Só uma dimensão
    fclose(f);

    res = scanftab_e_stack(&topo, "src/tabuleiros/tabuleiro_dim_invalidas.txt", linhas, colunas, tabuleiro);
    if (res != 2) CU_FAIL("Esperava erro ao ler dimensões do tabuleiro (código 2)");
    remove("src/tabuleiros/tabuleiro_dim_invalidas.txt");

    // Caso 3: Linhas incompletas no tabuleiro
    f = fopen("src/tabuleiros/tabuleiro_linhas_invalidas.txt", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "2 2\nab\n"); // Falta uma linha
    fclose(f);

    res = scanftab_e_stack(&topo, "src/tabuleiros/tabuleiro_linhas_invalidas.txt", linhas, colunas, tabuleiro);
    if (res != 3) CU_FAIL("Esperava erro ao ler linha incompleta do tabuleiro (código 3)");
    remove("src/tabuleiros/tabuleiro_linhas_invalidas.txt");
}

void teste_resolver_tabuleiro_valido(void) {
    char tabuleiro[3][3] = {
        {'a', 'b', 'c'},
        {'b', 'c', 'a'},
        {'c', 'a', 'b'}
    };

    Undo topo = NULL;
    resolve_aux(&topo, 3, 3, tabuleiro);

    // Verifica que todas as letras estão em maiúscula ou '#'
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            CU_ASSERT(isupper(tabuleiro[i][j]) || tabuleiro[i][j] == '#');
        }
    }

    // Verifica unicidade na linha 0 (A, B, C esperados)
    CU_ASSERT(tabuleiro[0][0] != tabuleiro[0][1]);
    CU_ASSERT(tabuleiro[0][1] != tabuleiro[0][2]);
    CU_ASSERT(tabuleiro[0][0] != tabuleiro[0][2]);

    free_undo(topo);
}

void teste_resolver_tabuleiro_sem_solucao(void) {
    char tabuleiro[2][2] = {
        {'a', 'a'},
        {'a', 'a'}
    };

    char copia[2][2];
    memcpy(copia, tabuleiro, sizeof(tabuleiro));

    Undo topo = NULL;
    resolve_aux(&topo, 2, 2, tabuleiro);

    // Verifica que o tabuleiro não foi resolvido (espera-se nenhuma mudança)
    if (memcmp(copia, tabuleiro, sizeof(tabuleiro)) != 0) {
        CU_FAIL("cmd_resolver modificou um tabuleiro sem solução.");
    }

    free_undo(topo);
}

void teste_resolver_tabuleiro_ja_resolvido(void) {
    char tabuleiro[2][2] = {
        {'A', 'B'},
        {'B', 'A'}
    };

    char copia[2][2];
    memcpy(copia, tabuleiro, sizeof(tabuleiro));

    Undo topo = NULL;
    resolve_aux(&topo, 2, 2, tabuleiro);

    CU_ASSERT(memcmp(tabuleiro, copia, sizeof(tabuleiro)) == 0);

    free_undo(topo);
}

void test_check_partial(void) {
    // Cenário 1: Letra única numa linha e coluna
    char board1[3][3] = {
        {'A', '.', '.'},
        {'.', '.', '.'},
        {'.', '.', '.'}
    };
    CU_ASSERT(check_partial(0, 0, 3, 3, board1) == 1);

    // Cenário 2: Letra repetida na mesma linha
    char board2[3][3] = {
        {'A', '.', 'A'},
        {'.', '.', '.'},
        {'.', '.', '.'}
    };
    CU_ASSERT(check_partial(0, 0, 3, 3, board2) == 0); // Deve falhar por duplicação na linha

    // Cenário 3: Letra repetida na mesma coluna
    char board3[3][3] = {
        {'A', '.', '.'},
        {'.', '.', '.'},
        {'A', '.', '.'}
    };
    CU_ASSERT(check_partial(0, 0, 3, 3, board3) == 0); // Deve falhar por duplicação na coluna

    // Cenário 4: '#' com adjacente '#' (inválido)
    char board4[3][3] = {
        {'.', '#', '.'},
        {'.', '#', '.'},
        {'.', '.', '.'}
    };
    CU_ASSERT(check_partial(1, 1, 3, 3, board4) == 0);

    // Cenário 5: '#' isolado (válido)
    char board5[3][3] = {
        {'.', '.', '.'},
        {'.', '#', '.'},
        {'.', '.', '.'}
    };
    CU_ASSERT(check_partial(1, 1, 3, 3, board5) == 1);
}


void test_connected_whites(void) {
    // Cenário 1: Nenhuma célula branca
    char board1[3][3] = {
        {'.', '.', '.'},
        {'.', '.', '.'},
        {'.', '.', '.'}
    };
    CU_ASSERT(connected_whites(3, 3, board1) == 1); // Nada para conectar, então é válido

    // Cenário 2: Um único bloco branco
    char board2[3][3] = {
        {'.', '.', '.'},
        {'.', 'A', '.'},
        {'.', '.', '.'}
    };
    CU_ASSERT(connected_whites(3, 3, board2) == 1); // Um só bloco é considerado conectado

    // Cenário 3: Blocos conectados em cruz
    char board3[3][3] = {
        {'.', 'A', '.'},
        {'A', 'B', 'A'},
        {'.', 'A', '.'}
    };
    CU_ASSERT(connected_whites(3, 3, board3) == 1); // Tudo está conectado

    // Cenário 4: Blocos separados
    char board4[3][3] = {
        {'A', '.', '.'},
        {'.', '.', '.'},
        {'.', '.', 'B'}
    };
    CU_ASSERT(connected_whites(3, 3, board4) == 0); // 'A' e 'B' não estão conectados

    // Cenário 5: Grande cluster conectado
    char board5[4][4] = {
        {'A', 'B', '.', '.'},
        {'.', 'C', 'D', '.'},
        {'.', '.', 'E', 'F'},
        {'.', '.', '.', '.'}
    };
    CU_ASSERT(connected_whites(4, 4, board5) == 1); // Todas brancas estão conectadas

    // Cenário 6: Cluster com uma branca desconectada
    char board6[4][4] = {
        {'A', 'B', '.', '.'},
        {'.', '.', '.', '.'},
        {'.', '.', '.', '.'},
        {'.', '.', '.', 'C'}
    };
    CU_ASSERT(connected_whites(4, 4, board6) == 0); // 'C' está isolada
}


void test_normalizar_tabuleiro(void) {
    int linhas = 3, colunas = 3;

    char original[3][3] = {
        {'A', 'B', 'C'},
        {'D', 'E', 'F'},
        {'G', 'H', 'I'}
    };

    // Cria o topo da pilha de undos
    Undo topo = NULL;

    // Aplica algumas mudanças simulando jogadas
    push_undo(&topo, 0, 0, 'a', 0); 
    push_undo(&topo, 0, 1, 'b', 0); 
    push_undo(&topo, 0, 2, 'c', 0); 
    push_undo(&topo, 1, 0, 'd', 0); 
    push_undo(&topo, 1, 1, 'e', 0); 
    push_undo(&topo, 1, 2, 'f', 0); 
    push_undo(&topo, 2, 0, 'g', 0); 
    push_undo(&topo, 2, 1, 'h', 0); 
    push_undo(&topo, 2, 2, 'i', 0);


    // Chamada da função a testar
    normalizar_tabuleiro(linhas, colunas, original, &topo, original);

    // Verificações
    CU_ASSERT(original[0][0] == 'a'); 
    CU_ASSERT(original[0][1] == 'b'); 
    CU_ASSERT(original[0][2] == 'c'); 
    CU_ASSERT(original[1][0] == 'd');
    CU_ASSERT(original[1][1] == 'e'); 
    CU_ASSERT(original[1][2] == 'f');
    CU_ASSERT(original[2][0] == 'g'); 
    CU_ASSERT(original[2][1] == 'h');
    CU_ASSERT(original[2][2] == 'i'); 

    free_undo(topo);

}

void test_cmd_resolver(void) {
    Undo topo = NULL;

char board[20][20] = {
    {'q','b','n','j','t','j','m','s','f','m','k','d','o','p','h','o','m','a','l','r'},
    {'l','g','d','h','c','t','m','i','o','n','q','o','j','j','g','b','r','s','p','f'},
    {'t','l','l','d','j','r','s','q','j','g','m','g','a','c','b','h','o','e','q','i'},
    {'e','e','r','j','a','b','f','j','s','d','j','q','r','n','n','t','p','i','m','h'},
    {'a','a','h','k','l','m','e','c','b','l','d','s','p','n','d','q','p','o','m','g'},
    {'s','r','m','a','q','e','b','g','k','q','n','j','k','l','m','n','g','c','t','d'},
    {'f','c','a','t','g','r','d','k','b','o','l','a','m','i','q','r','j','e','n','g'},
    {'i','l','e','d','n','c','k','p','o','q','j','r','d','i','f','h','l','b','h','a'},
    {'j','p','p','i','h','f','f','s','t','s','c','b','r','k','r','j','b','m','g','d'},
    {'b','g','j','o','r','q','l','g','h','r','k','m','d','s','e','a','i','n','n','t'},
    {'k','l','p','f','i','o','h','a','m','d','s','r','e','g','e','h','h','d','r','f'},
    {'s','m','k','c','f','b','t','g','e','i','a','l','q','n','j','i','h','p','s','k'},
    {'g','f','h','m','n','i','h','b','e','b','d','k','c','q','n','p','k','s','a','l'},
    {'m','t','m','k','o','s','p','n','e','c','q','h','k','d','i','e','q','j','f','m'},
    {'c','m','i','b','n','a','j','f','g','s','r','p','n','o','s','m','s','l','n','q'},
    {'a','o','r','n','s','c','p','e','a','h','g','t','l','b','i','j','m','m','q','c'},
    {'m','q','s','c','j','d','k','l','p','a','f','f','c','h','j','n','e','r','i','n'},
    {'f','n','g','s','m','h','r','c','l','o','b','a','f','e','c','k','e','i','j','o'},
    {'o','e','m','p','n','n','b','r','c','k','h','s','i','b','a','d','f','q','q','j'},
    {'r','j','t','r','l','d','o','m','k','e','q','p','s','d','a','c','p','k','d','p'}
};

    char expected[20][20] = {
    {'Q','B','N','#','T','J','M','#','F','#','K','D','#','P','H','O','#','A','L','R'},
    {'L','#','D','H','C','T','#','I','O','N','Q','#','J','#','G','B','R','S','P','#'},
    {'T','L','#','D','#','R','S','Q','J','#','M','G','A','C','B','#','O','E','#','I'},
    {'E','#','R','#','A','B','F','#','S','D','J','Q','#','N','#','T','P','#','M','H'},
    {'#','A','H','K','#','M','E','C','#','L','#','S','P','#','D','Q','#','O','#','G'},
    {'S','R','#','A','Q','E','B','#','K','#','N','J','#','L','M','#','G','C','T','D'},
    {'F','C','A','T','G','#','D','K','B','O','L','#','M','I','Q','R','J','#','N','#'},
    {'I','#','E','#','N','C','K','P','#','Q','#','R','D','#','F','#','L','B','H','A'},
    {'#','P','#','I','H','F','#','S','T','#','C','B','R','K','#','J','#','M','G','#'},
    {'B','G','J','O','#','Q','L','#','H','R','#','M','#','S','E','A','I','N','#','T'},
    {'K','#','P','#','I','O','#','A','M','#','S','#','E','G','#','H','#','D','R','F'},
    {'#','M','K','C','F','#','T','G','#','I','A','L','Q','#','J','#','H','P','S','#'},
    {'G','F','#','M','#','I','H','#','E','B','D','#','C','Q','N','P','K','#','A','L'},
    {'#','T','M','#','O','S','#','N','#','C','#','H','K','D','#','E','Q','J','F','#'},
    {'C','#','I','B','#','A','J','F','G','#','R','#','N','O','S','M','#','L','#','Q'},
    {'A','O','#','N','S','#','P','E','#','H','G','T','L','#','I','#','M','#','Q','C'},
    {'M','Q','S','#','J','D','#','L','P','A','#','F','#','H','#','N','E','R','I','#'},
    {'#','N','G','S','M','H','R','#','L','#','B','A','F','E','C','K','#','I','J','O'},
    {'O','E','#','P','#','N','#','R','C','K','H','#','I','B','#','D','F','Q','#','J'},
    {'#','J','T','R','L','#','O','M','#','E','#','P','S','#','A','C','#','K','D','#'}
};


    // Executa cmd_resolver que deve alocar e atualizar topo
    cmd_resolver(&topo, 20, 20, board);

    // Verifica tabuleiro
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != expected[i][j]) {
                printf("Esperado board[%d][%d] = %c, mas encontrou %c\n", i, j, expected[i][j], board[i][j]);
                CU_FAIL("cmd_resolver falhou na conversão das letras minúsculas.");
                free_undo(topo);  // Liberar antes de sair
                return;
            }
        }
    }

    CU_PASS("cmd_resolver converteu corretamente todas as letras minúsculas esperadas.");

    free_undo(topo);  // Liberar toda memória alocada para Undo
}
