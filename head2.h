#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <limits.h>
#include "lib-utf8.h"

#define MAX_LINE 70
#define MAX_PAL 50
#define C_RED       "\033[31m"
#define C_BLUE      "\033[34m"
#define C_GRAY      "\033[37m"
#define NONE        "\033[0m"


struct palavra {
    char pal[MAX_PAL];
    int ind;
    struct palavra *p_seguinte;
};

struct no {
    struct palavra p;
    struct no *left;
    struct no *right;
};

struct arvore_binaria {
    struct no *raiz;
};

bool pede_ficheiro(char *nome, char *nome_original, int *aux);
bool insere_dados(char *nome, struct arvore_binaria *arvore);
int busca_num();
void pesquisa_palavras(struct arvore_binaria *ab);
void imprime_tabela(struct no *np);
void mostra_palavras(struct arvore_binaria *ab, FILE *fich);