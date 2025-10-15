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

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Suite Testes", 0, 0);

    CU_add_test(suite, "test_printmatriz", test_printmatriz);
    CU_add_test(suite, "test_push_and_desfazer", test_push_and_desfazer);
    CU_add_test(suite, "test_cmd_b", test_cmd_b);
    CU_add_test(suite, "test_gravar_jogo", test_gravar_jogo);
    CU_add_test(suite, "test_coordenada", test_coordenada);
    CU_add_test(suite, "test_cmd_v", test_cmd_v);
    CU_add_test(suite, "test_obter_dimensoes", test_obter_dimensoes);
    CU_add_test(suite, "test_scanftab_e_stack", test_scanftab_e_stack);
    CU_add_test(suite, "teste_resolver_tabuleiro_valido", teste_resolver_tabuleiro_valido);
    CU_add_test(suite, "teste_resolver_tabuleiro_sem_solucao", teste_resolver_tabuleiro_sem_solucao);
    CU_add_test(suite, "teste_resolver_tabuleiro_ja_resolvido", teste_resolver_tabuleiro_ja_resolvido);
    CU_add_test(suite, "test_cmd_a", teste_cmd_a);
    CU_add_test(suite, "test_verifica_riscadas", test_verifica_riscadas_converte_minuscula);
    CU_add_test(suite, "teste_cmd_a_grande", teste_cmd_a_grande);
    CU_add_test(suite, "test_cmd_r", test_cmd_r);
    CU_add_test(suite, "test_connected_whites", test_connected_whites);
    CU_add_test(suite, "test_check_partial", test_check_partial);
    CU_add_test(suite, "test_normalizar_tabuleiro", test_normalizar_tabuleiro);
    CU_add_test(suite, "test_cmd_resolver", test_cmd_resolver);

    CU_add_test(suite, "test_cmd_b_fail", test_cmd_b_fail);
    CU_add_test(suite, "test_push_and_desfazer_fail", test_push_and_desfazer_fail);
    CU_add_test(suite, "test_obter_dimensoes_fail", test_obter_dimensoes_fail);
    CU_add_test(suite, "test_gravar_jogo_fail", test_gravar_jogo_fail);
    CU_add_test(suite, "test_scanftab_e_stack_fail", test_scanftab_e_stack_fail);
    CU_add_test(suite, "test_coordenada_fail", test_coordenada_fail);
    CU_add_test(suite, "test_cmd_r_fail", test_cmd_r_fail);

    CU_add_test(suite, "test_nome_curto_sem_extensao", test_nome_curto_sem_extensao);
    CU_add_test(suite, "test_nome_com_extensao_errada", test_nome_com_extensao_errada);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}