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

void test_cmd_v(void) {
    char tab1[2][2] = {{'A', 'B'}, {'C', 'D'}};
    CU_ASSERT_EQUAL(cmd_v(2, 2, tab1), 0);

    char tab2[2][2] = {{'A', 'A'}, {'C', 'D'}};
    CU_ASSERT_TRUE(cmd_v(2, 2, tab2) > 0);

    char tab3[2][2] = {{'A', 'B'}, {'A', 'D'}};
    CU_ASSERT_TRUE(cmd_v(2, 2, tab3) > 0);

    char tab4[2][2] = {{'#', 'b'}, {'C', 'D'}};
    CU_ASSERT_TRUE(cmd_v(2, 2, tab4) > 0);

    char tab5[3][3] = {
        {'A', '#', '#'},
        {'#', '#', '#'},
        {'#', '#', 'B'}
    };
    CU_ASSERT_TRUE(cmd_v(3, 3, tab5) > 0);
}