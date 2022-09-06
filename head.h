#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <limits.h>
#include "lib-utf8.h"

#define MAX_PAL 50
#define C_RED       "\033[31m"
#define C_BLUE      "\033[34m"
#define NONE        "\033[0m"

bool pede_ficheiro(char *nome, int *aux);
bool guarda_ficheiro(FILE *fich_leitura, FILE *fich_escrita);