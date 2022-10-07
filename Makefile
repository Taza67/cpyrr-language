# Variables

MAKE = make

## Compilateur

CC = gcc
OPTIONS1 = -Wall
OPTIONS2 = -W -g -std=c89 -pedantic -O3
GL_CFLAGS = -lglut -lGLU -lGL
MATH_CFLAGS = -lm
MLV_CFLAGS = `pkg-config --cflags MLV`
MLV_LDFLAGS = `pkg-config --libs-only-other --libs-only-L MLV`
MLV_LDLIBS = `pkg-config --libs-only-l MLV`

## Génerateurs d'analyseurs lexicaux - syntaxiques

LEX = lex
YACC = bison
OPTIONS_YACC = -d -v -Wcounterexamples

## Répertoires

SRC_DIR = ./src
HED_DIR = ./inc
OBJ_DIR = ./obj
BIN_DIR = ./bin

LEX_YACC_DIR = ./lex_yacc

### Dossier pour les fichiers de sortie

OUT_DIR = ./default

## Dépendances

SRC = $(shell find $(SRC_DIR) -name "*.c")
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))
INC = -I$(HED_DIR)

## Exécutables

EXEC = cpyrr-cvm


# Fonctions

build:
	@$(MAKE) directories --no-print-directory
	@echo "- Building..."
	@$(MAKE) all --no-print-directory
	@echo "- Done"

rebuild:
	@$(MAKE) clean --no-print-directory
	@$(MAKE) build --no-print-directory

all:
	@make $(BIN_DIR)/$(EXEC) --no-print-directory

clean:
	@echo "- Cleaning..."
	@rm -rf *~ $(EXEC) $(OBJ_DIR) $(SRC_DIR)*~ $(BIN_DIR) $(OUT_DIR) $(LEX_YACC_DIR) log*
	@echo "- Cleaning done"

clean_output:
	@echo "- Cleaning output files..."
	@rm -rf $(OUT_DIR)
	@echo "- Cleaning done"

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(LEX_YACC_DIR)

.PHONY: build rebuild clean clean_output all directories


# Compilation

## Exécutables

$(BIN_DIR)/$(EXEC): $(LEX_YACC_DIR)/lex_yacc.tab.c $(LEX_YACC_DIR)/lex.yy.o $(OBJ)
	@echo "+ + Linking \"$(notdir $@)\"..."
	@$(CC) $^ -o $@ $(OPTIONS1)
	@echo "+ + Linking done"

## LEX - YACC

$(LEX_YACC_DIR)/lex_yacc.tab.c: lex_yacc.y
	@echo "+ + + Compiling \"$(notdir $<)\""
	@$(YACC) -b $(LEX_YACC_DIR)/lex_yacc $(OPTIONS_YACC) $^

$(LEX_YACC_DIR)/lex.yy.o: $(LEX_YACC_DIR)/lex.yy.c
	@echo "+ + + Compiling \"$(notdir $<)\""
	@$(CC) -o $@ -c $^

$(LEX_YACC_DIR)/lex.yy.c: lex_yacc.l
	@echo "+ + + Compiling \"$(notdir $<)\""
	@$(LEX) -o $@ $^

## Objets

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "+ + + Compiling \"$(notdir $<)\""
	@$(CC) -o $@ -c $(OPTIONS_CC2) $(INC) $<
