# Compilador
CC = gcc

# Flags de compilação padrão
CFLAGS = -Wall -Wextra -pedantic -O1 -g -std=c99 -fsanitize=address -fno-omit-frame-pointer

# Flags para cobertura de código
COVFLAGS = -fprofile-arcs -ftest-coverage

# Flags para AddressSanitizer (opcional)
SANFLAGS = -fsanitize=address -fno-omit-frame-pointer

# Diretorias
SRC_DIR = src
TESTS_DIR = tests
BUILD_DIR = build

# Arquivos fonte
SRC_JOGO = $(SRC_DIR)/main.c $(SRC_DIR)/funcoes.c $(SRC_DIR)/resolver.c \
           $(SRC_DIR)/ajuda.c $(SRC_DIR)/brancas_riscar.c $(SRC_DIR)/ler_gravar.c \
           $(SRC_DIR)/verificar.c $(SRC_DIR)/resolve_aux.c
SRC_TESTE = $(TESTS_DIR)/maintestes.c \
            $(TESTS_DIR)/testes.c \
            $(TESTS_DIR)/testes_a.c \
            $(TESTS_DIR)/testes_b.c \
            $(TESTS_DIR)/testes_coordenada.c \
            $(TESTS_DIR)/testes_d.c \
            $(TESTS_DIR)/testes_gravar.c \
            $(TESTS_DIR)/testes_r.c \
            $(TESTS_DIR)/testes_v.c

# Objetos
OBJ_JOGO = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_JOGO))
OBJ_TESTE = $(patsubst $(TESTS_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_TESTE))

# Executáveis
JOGO = $(BUILD_DIR)/jogo
TESTES = $(BUILD_DIR)/testes

# Alvo padrão
all: $(BUILD_DIR) clean $(JOGO) $(TESTES)

# Criação da pasta build se não existir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilação do jogo
$(JOGO): $(OBJ_JOGO)
	$(CC) $(CFLAGS) $(COVFLAGS) -o $@ $^

# Compilação dos testes com link para CUnit
$(TESTES): $(OBJ_TESTE) $(filter-out $(BUILD_DIR)/main.o, $(OBJ_JOGO))
	$(CC) $(CFLAGS) $(COVFLAGS) -o $@ $^ -lcunit

# Regra genérica para objetos
# %.o: %.c %.h
# 		$(CC) $(CFLAGS) $(COVFLAGS) -c $< -o $@
# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/funcoes.h
# 	$(CC) $(CFLAGS) $(COVFLAGS) -c $< -o $@

# $(BUILD_DIR)/testes.o: $(TESTS_DIR)/testes.c $(SRC_DIR)/funcoes.h
# 	$(CC) $(CFLAGS) $(COVFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(COVFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.c
	$(CC) $(CFLAGS) $(COVFLAGS) -c $< -o $@


jogo: $(JOGO)

testes: $(TESTES)

# Rodar o jogo 
executar: $(JOGO)
	./$(JOGO)

# Rodar os testes com input (> /dev/null ignora os printfs)
testar: $(TESTES)
	./$(TESTES)

# Cobertura de código com gcov
coverage: clean
	$(MAKE) CFLAGS="-Wall -Wextra -pedantic -O0 -g -std=c99" COVFLAGS="-fprofile-arcs -ftest-coverage" $(TESTES)
	./$(TESTES)
#	gcov -b -c $(SRC_DIR)/funcoes.c $(SRC_DIR)/resolver.c
	gcovr --exclude='.*main.c' --filter src/ --branches

# Ǵera ficheiro html, é preciso ter o gcovr instalado (sudo apt install gcovr)
html-coverage: clean all
	./$(TESTES)
	gcovr --html --html-details -o coverage/cobertura.html --filter src/ --exclude='.*main.c'
	@xdg-open coverage/cobertura.html || open coverage/cobertura.html

# Limpeza
.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.gcno $(BUILD_DIR)/*.gcov $(JOGO) $(TESTES)

#rm -f *.o *.gcda *.gcno *.gcov $(JOGO) $(TESTES)