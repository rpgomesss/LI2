#ifndef TESTES_H
#define TESTES_H

#include "../src/funcoes.h" 

void test_printmatriz(void);
void test_push_and_desfazer(void);
void test_push_and_desfazer_fail(void);
void test_cmd_b(void);
void test_cmd_b_fail(void);
void test_cmd_r(void);
void test_cmd_r_fail(void);
void test_gravar_jogo(void);
void test_gravar_jogo_fail(void);
void test_lerjogo(void);
void test_lerjogo_fail(void);
void test_coordenada(void);
void test_coordenada_fail(void);
void test_cmd_v(void);
void test_obter_dimensoes(void);
void test_obter_dimensoes_fail(void);
void test_scanftab_e_stack(void);
void free_undo(Undo topo);  // Se usada em vários ficheiros
void teste_cmd_a(void);
void teste_cmd_a_grande(void);
void test_verifica_riscadas_converte_minuscula(void);
void teste_resolver_tabuleiro_ja_resolvido(void);
void teste_resolver_tabuleiro_sem_solucao(void);
void teste_resolver_tabuleiro_valido(void);
void test_scanftab_e_stack_fail(void);
void test_nome_com_extensao_errada(void);
void test_nome_curto_sem_extensao(void);
void test_cmd_r(void);
void test_cmd_r_fail(void);
void test_check_partial(void);
void test_connected_whites(void);
void test_normalizar_tabuleiro(void);
void test_cmd_resolver(void);

#endif 
