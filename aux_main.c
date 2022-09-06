#include "head.h"

static void flush_in();

bool pede_ficheiro(char *nome, int *aux) {
    FILE *fich = NULL;
    char *s;

    s = malloc(MAX_PAL * MB_CUR_MAX * sizeof(char) + 1);
    if (s == NULL) {
        fprintf(stderr, "Sem espaço para alocar memória\n");
        return false;
    }

    printf("==== LEITURA DO TEXTO A PROCESSAR ====\n");

    while (fich == NULL) {
        printf("-> Nome do Ficheiro?\n");

        fgets(s, MAX_PAL * MB_CUR_MAX * sizeof(char), stdin);
        if (strlen(s) == MAX_PAL * MB_CUR_MAX * sizeof(char)-1){
            flush_in();
        }
        if (strtok(s, " \n\r\t\v") == NULL) {
            printf("%sExprimente escrever alguma coisa!%s\n", C_BLUE, NONE);
        }
        else if (strtok(NULL, " \n\r\t\v") != NULL){
            printf("%sNao insira mais do que um nome de um texto!%s\n\n", C_RED, NONE);
        } else {
            if (strcasecmp(s, "SAIR") == 0) {
                free(s);
                *aux = 1;
                return true;
            }
            fich = fopen(s, "r");
            if (fich != NULL) {
                if ((s[0] == 't' || s[0] == 'T') && (s[1] == 'a' || s[1] == 'A') && (s[2] == 'b' || s[2] == 'B') &&
                         s[3] == '_') {
                    printf("%sFicheiros com o formato <tab_> no inicio não são aceites%s\n\n", C_RED, NONE);
                    fich = NULL;
                }
            } else
                printf("%sO Ficheiro nao existe%s\n\n", C_RED, NONE);
        }
    }
    strcpy(nome, s);
    free(s);
    if (fclose(fich) == EOF) {
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return false;
    }
    return true;
}

bool guarda_ficheiro(FILE *fich_leitura, FILE *fich_escrita){
    int c;
    char ign[] = " .():;,!?-_<>+*\t\v\'\"\n\r";

    int i = 0;
    int ind;
    bool ver = true;
    char *string;

    string = malloc(MAX_PAL * MB_CUR_MAX * sizeof(char) + 1);
    if (string == NULL) {
        fprintf(stderr, "Sem espaço para alocar memória\n");
        return 1;
    }

    if (fseek(fich_leitura, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Erro no fseek\n");
        return 1;
    }
    while ((c = fgetc(fich_leitura)) != EOF) {
        ver = true;
        for (int j = 0; j < (int) strlen_u8(ign); ++j) {
            if (c == ign[j]) {
                ver = false;
            }
        }
        if (ver) {
            string[i] = (char) c;
            i++;
        } else {
            if (strlen_u8(string) > 2) {
                
                ind = (int) (ftell(fich_leitura) - strlen(string) - 1);
                fprintf(fich_escrita, "%s+%d\n", string, ind);
            }

            memset(string, 0, MAX_PAL * MB_CUR_MAX * sizeof(char) + 1);
            i = 0;
        }
    }
    printf("FICHEIRO BEM GUARDADO\n");

    free(string);
    return true;
}

void flush_in() {
    int ch;
    do {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}