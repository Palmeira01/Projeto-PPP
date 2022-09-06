#include "head2.h"

int main() {
    setlocale(LC_ALL,"pt_PT.utf8");
    char *nome;
    char *nome_tab;
    int aux = 0;

    nome = malloc(MAX_PAL * MB_CUR_MAX *sizeof(char) + 1);
    if (nome == NULL){
        fprintf(stderr, "Sem espaço para alocar memória\n");
        return 1;
    }

    nome_tab = malloc(MAX_PAL * MB_CUR_MAX *sizeof(char) + 1);
    if (nome_tab == NULL){
        fprintf(stderr, "Sem espaço para alocar memória\n");
        return 1;
    }

    if (!pede_ficheiro(nome_tab, nome, &aux))
        return 1;

    if (aux == 1){
        printf("Bye Bye!!\n");
        return 0;
    }

    struct arvore_binaria *arvore;
    arvore = (struct arvore_binaria *) malloc(sizeof(struct arvore_binaria));
    if (arvore == NULL){
        fprintf(stderr, "Sem espaço para alocar memoria\n");
        return 1;
    }
    arvore->raiz = NULL;

    if (!insere_dados(nome_tab, arvore)) {
        return 1;
    }

    int escolha;
    FILE *fich;

    fich = fopen(nome, "r");
    if (fich == NULL) {
        fprintf(stderr, "Erro ao abrir ficheiro tab_\n");
        return 1;
    }
    while (true) {
        printf("\n\n\n\n---------------------------------------------\n");
        printf("\t\t\t\t\tMENU");
        printf("\n---------------------------------------------\n");
        printf("1. Procurar uma palavra\n");
        printf("2. Procurar palavras com termo de pesquisa\n");
        printf("0. Saír\n\n");
        printf("Escolha: ");
        escolha = busca_num();
        if (escolha == 1) {
            mostra_palavras(arvore, fich);
        } else if (escolha == 2) {
            pesquisa_palavras(arvore);
        }else if (escolha == 0) {
            printf("Bye Bye!!");
            break;
        } else if (escolha == -2){
            printf("%sExprimente escrever alguma coisa!%s\n", C_BLUE, NONE);
        }
        else {
            printf("Comando inválido!\n");
        }
    }
    free(nome);
    free(nome_tab);
    free(arvore);
    if (fclose(fich) == EOF) {
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return 1;
    }
    fich = NULL;
    return 0;
}