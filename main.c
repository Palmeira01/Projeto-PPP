#include "head.h"

int main() {
    setlocale(LC_ALL, "pt_PT.utf8");

    FILE *fich = NULL;
    FILE *fich2;

    char *nome_orig;
    char *nome;

    nome_orig = malloc(MAX_PAL * MB_CUR_MAX * sizeof(char) + 1);
    if (nome_orig == NULL) {
        fprintf(stderr, "Sem espaço para alocar memória\n");
        return 1;
    }
    nome = malloc(MAX_PAL * MB_CUR_MAX * sizeof(char) + 1);
    if (nome == NULL) {
        fprintf(stderr, "Sem espaço para alocar memória\n");
        return 1;
    }

    int aux = 0;
    if (!pede_ficheiro(nome_orig, &aux)) {
        return 1;
    }
    if (aux != 0){
        printf("Bye Bye!\n");
        return 0;
    }

    strcpy(nome, "tab_");
    strcat(nome, nome_orig);

    fich = fopen(nome_orig, "r");
    if (fich == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro %s\n", nome_orig);
        return false;
    }

    fich2 = fopen(nome, "w");
    if (fich2 == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro %s\n", nome);
        return false;
    }

    if (!guarda_ficheiro(fich, fich2)){
        return 1;
    }

    free(nome_orig);
    free(nome);

    if (fclose(fich) == EOF){
        fprintf(stderr, "Erro ao fechar o ficheiro de leitura\n");
        return 1;
    }
    fich = NULL;
    if (fclose(fich2) == EOF){
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return 1;
    }
    fich2 = NULL;
    return 0;
}