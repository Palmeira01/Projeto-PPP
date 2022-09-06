#include "head2.h"

static int converte_num(char *numero);
static bool colocar(struct arvore_binaria *ab, char *palavra_t, int indice);
static struct no *addtree(struct no *pr, struct no *pn);
static void coloca_lista(struct no *pr, struct no *pn);
static struct palavra *addlist(struct palavra *pr, struct palavra *pn);
static void procura_pal(FILE *fich, struct no *pr, char *s, int *auxiliar);
static void busca_contexto(FILE *fich, struct palavra *pal);
static bool ignora_carac(char c);
static void pesquisa_palavras_arvore(struct no *pn, char *pal_pesq, int *auxiliar);
static void flush_in();

bool pede_ficheiro(char *nome, char *nome_original, int *aux) {
    FILE *fich = NULL;
    char *s;

    s = malloc(MAX_PAL * MB_CUR_MAX * sizeof(char) + 1);
    if (s == NULL) {
        fprintf(stderr, "Sem espaço para alocar memória\n");
        return false;
    }

    printf("\n======== TEXTO JÁ PROCESSADO ========\n");

    while (fich == NULL) {
        printf("Nome do Ficheiro? (<tab_>)\n");

        fgets(s, MAX_PAL * MB_CUR_MAX * sizeof(char), stdin);
        if (strlen(s) == MAX_PAL * MB_CUR_MAX * sizeof(char)-1){
            flush_in();
        }
        if (strtok(s, " \n\r\t\v") == NULL) {
            printf("%sExprimente escrever alguma coisa!%s\n", C_BLUE, NONE);
        } else if (strtok(NULL, " \n\r\t\v") != NULL){
            printf("%sNao insira mais do que um nome de um texto!%s\n\n", C_RED, NONE);
        } else {
            if (strcasecmp(s, "SAIR") == 0) {
                free(s);
                *aux = 1;
                return true;
            }
            fich = fopen(s, "r");
            if (fich == NULL) {
                printf("%sO Ficheiro nao existe%s\n\n", C_RED, NONE);
            } else if (s[0] != 't' || s[1] != 'a' || s[2] != 'b' || s[3] != '_') {
                printf("%sFicheiro invalido%s\n\n", C_RED, NONE);
                fich = NULL;
            }

        }
    }
    strcpy(nome, s);
    free(s);
    if (fclose(fich) == EOF) {
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return false;
    }

    FILE *ficheiro;
    int j = 4;
    int z;
    for (z = j; z < (int) strlen(nome); ++z) {
        nome_original[z - j] = nome[z];
    }
    nome_original[z] = '\0';

    ficheiro = fopen(nome_original, "r");
    if (ficheiro == NULL) {
        fprintf(stderr, "Ficheiro original não encontrado\n");
        return false;
    }
    if (fclose(ficheiro) == EOF) {
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return false;
    }
    ficheiro = NULL;
    return true;
}


bool insere_dados(char *nome, struct arvore_binaria *arvore) {
    FILE *fich;
    //abre o ficheiro
    fich = fopen(nome, "r");
    if (fich == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro.\n");
        return false;
    }

    int indice;
    char *palavra_t;
    char *linha;

    palavra_t = malloc(MAX_PAL * MB_CUR_MAX * sizeof(char) + 1);
    if (palavra_t == NULL) {
        fprintf(stderr, "Sem espaço para alocar memória\n");
        return false;
    }
    linha = malloc(MAX_LINE * MB_CUR_MAX * sizeof(char) + 1);
    if (linha == NULL) {
        fprintf(stderr, "Sem espaço para alocar memória\n");
        return false;
    }

    while (fscanf(fich, "%s", linha) != EOF) {
        strcpy(palavra_t, strtok(linha, "+\n\r"));
        indice = converte_num(strtok(NULL, "+\n\r"));

        if (!colocar(arvore, palavra_t, indice)) {
            printf("Não há espaço para inserir.\n");
            return false;
        }
    }

    free(palavra_t);
    free(linha);
    if (fclose(fich) == EOF) {
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return false;
    }
    fich = NULL;
    return true;
}

static int converte_num(char *numero) {
    int num = 0;
    for (int i = 0; i < (int) strlen(numero); ++i) {
        num *= 10;
        num += numero[i] - '0';
    }
    return num;
}

static bool colocar(struct arvore_binaria *ab, char *palavra_t, int indice) {
    struct no *aux;
    aux = (struct no *) malloc(sizeof(struct no));
    if (aux == NULL)
        return false;

    strcpy(aux->p.pal, palavra_t);
    aux->p.ind = indice;
    aux->p.p_seguinte = NULL;
    aux->left = aux->right = NULL;

    ab->raiz = addtree(ab->raiz, aux);
    return true;
}

static struct no *addtree(struct no *pr, struct no *pn) {
    int cond;
    char nome1[MAX_PAL * MB_CUR_MAX * sizeof(char) + 1], nome2[MAX_PAL * MB_CUR_MAX * sizeof(char) + 1];
    if (pr == NULL) {
        pr = pn;
    } else {
        strtobase_u8(nome1, pn->p.pal);
        strtobase_u8(nome2, pr->p.pal);
        if ((cond = strcmp(nome1, nome2)) > 0) {
            pr->right = addtree(pr->right, pn);
        } else if (cond < 0)
            pr->left = addtree(pr->left, pn);
        else {
            coloca_lista(pr, pn);
        }
    }
    return pr;
}

static void coloca_lista(struct no *pr, struct no *pn) {
    struct palavra *aux;
    aux = (struct palavra *) malloc(sizeof(struct palavra));
    if (aux == NULL)
        return;

    strcpy(aux->pal, pn->p.pal);
    aux->ind = pn->p.ind;
    aux->p_seguinte = NULL;
    free(pn);

    pr->p.p_seguinte = addlist(pr->p.p_seguinte, aux);
}

static struct palavra *addlist(struct palavra *pr, struct palavra *pn) {
    if (pr == NULL) {
        pr = pn;
    } else {
        pr->p_seguinte = addlist(pr->p_seguinte, pn);
    }
    return pr;
}

void imprime_tabela(struct no *np) {
    if (np == NULL) {
        return;
    }
    imprime_tabela(np->left);
    printf("%s -> %d\n", np->p.pal, np->p.ind);
    imprime_tabela(np->right);
}

int busca_num() {
    int num;
    int tam = MAX_LINE * MB_CUR_MAX * sizeof(char);
    char s[tam + 1];

    fgets(s, tam, stdin);

    if ((int)strlen(s) == tam){
        flush_in();
    }

    if (strtok(s, " \n\r\t\v") == NULL){
        return -2;
    }
    else if (((int) strlen(s)) == 1) {
        num = (int) (s[0] - '0');
        if (num >= 0 && num <= 9) {
            return num;
        }
    }

    return -1;
}

void mostra_palavras(struct arvore_binaria *ab, FILE *fich) {
    char s[MAX_PAL * MB_CUR_MAX * sizeof(char) + 1], s1[MAX_PAL * MB_CUR_MAX * sizeof(char) + 1];
    int auxiliar = 0;

    printf("\nInsira a palavra que pretende procurar: ");
    fgets(s, MAX_PAL, stdin);
    if (strlen(s) == MAX_PAL-1){
        flush_in();
    }
    if (strtok(s, " \n\r\t\v") == NULL){
        printf("%sExprimente escrever alguma coisa da proxima vez!%s\n", C_BLUE, NONE);
        return;
    }
    if (strtok(NULL, " \n\r\t\v") != NULL){
        printf("%sNao insira mais do que uma palavra!%s\n", C_RED, NONE);
        return;
    }
    strtobase_u8(s1, s);
    printf("\n\nLista com a palavra <%s> nos diferentes contextos:\n", s);
    procura_pal(fich, ab->raiz, s1, &auxiliar);
    if (auxiliar == 0) {
        printf("\t<Lista vazia>\n");
    }
}

static void procura_pal(FILE *fich, struct no *pr, char *s, int *auxiliar) {
    if (pr == NULL) {
        return;
    }
    char s1[MAX_PAL * MB_CUR_MAX * sizeof(char) + 1];
    strtobase_u8(s1, pr->p.pal);
    int ind;
    if ((ind = strcmp(s1, s)) == 0) {
        *auxiliar = 1;
        busca_contexto(fich, &pr->p);
    } else if (ind < 0) {
        procura_pal(fich, pr->right, s, auxiliar);
    } else {
        procura_pal(fich, pr->left, s, auxiliar);
    }
}

static void busca_contexto(FILE *fich, struct palavra *pal) {
    int i = 11;
    char c;

    int aux = 0;
    if (fseek(fich, pal->ind, SEEK_SET) != 0) {
        printf("Erro no fseek 0\n");
    }

    while (i != 0) {
        if (ftell(fich) == 0) {
            --i;
            break;
        }
        if (fseek(fich, -1, SEEK_CUR) != 0) {
            printf("Erro no fseek 1\n");
        }
        fscanf(fich, "%c", &c);
        if (fseek(fich, -1, SEEK_CUR) != 0) {
            printf("Erro no fseek 2\n");
        }
        if (!ignora_carac(c)) {
            if (aux != 1) {
                --i;
                aux = 1;
            }
        } else
            aux = 0;
    }

    char st[MAX_PAL * MB_CUR_MAX * sizeof(char) + 1];
    bool ver;
    printf("\t-> \"");
    for (int j = i; j < 16; ++j) {
        ver = false;
        if (fscanf(fich, "%s", st) == EOF) {
            break;
        }
        if (j != i) {
            printf(" ");
        }

        for (int k = 0; k < (int)strlen(st); ++k) {
            if (ignora_carac(st[k])){
                ver = true;
            }
        }
        if (ver == false){
            j--;
        }

        if (j != 10) {
            printf("%s", st);
        } else
            printf("%s%s%s", C_GRAY, st, NONE);

    }
    printf("\"\n");
    if (pal->p_seguinte == NULL) {
        return;
    } else {
        busca_contexto(fich, pal->p_seguinte);
    }
}

static bool ignora_carac(char c) {
    char ign[] = " .():;,!?-_<>+*\t\v\'\"\n\r";
    for (int i = 0; i < (int) strlen(ign); ++i) {
        if (c == ign[i]) {
            return false;
        }
    }
    return true;
}

void pesquisa_palavras(struct arvore_binaria *ab) {
    char s[MAX_PAL * MB_CUR_MAX * sizeof(char) + 1], s1[MAX_PAL * MB_CUR_MAX * sizeof(char) + 1], *s2;
    char c[2];
    int escolha;
    int auxiliar;
    char dicio[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int ind_ini, ind_fin;

    while (true) {
        auxiliar = 0;
        printf("\n\nEscolha o tipo de pesquisa que pretende fazer:\n");
        printf("\t1. Pesquisa de palavras com as iniciais especificas\n");
        printf("\t2. Pesquisa de um intervalo de letras\n");
        printf("\t0. Voltar\n");

        printf("Escolha: ");
        escolha = busca_num();

        if (escolha == -2){
            printf("%sExprimente escolher uma das opções!%s\n", C_BLUE, NONE);
        }
        else if (escolha == 1) {
            printf("\nInsira o(s) termo(s) de pesquisa:\n(Nota: caso pretenda colocar vários termos de pesquisa use os espaços como separadores)\n ");
            fgets(s, MAX_PAL * MB_CUR_MAX * sizeof(char), stdin);
            if (strlen(s) == MAX_PAL * MB_CUR_MAX * sizeof(char) - 1){
                flush_in();
            }
            if (strtok(s, "\n\r") == NULL){
                printf("%sExprimente escrever alguma coisa da proxima vez!%s\n", C_BLUE, NONE);
            } else {
                strtobase_u8(s1, s);
                s2 = strtok(s1, " ");
                while (s2 != NULL) {
                    auxiliar = 0;
                    printf("\nLista de palavras com as iniciais <%s>:\n", s2);
                    pesquisa_palavras_arvore(ab->raiz, s2, &auxiliar);
                    if (auxiliar == 0) {
                        printf("\t<Lista vazia>\n");
                    }
                    s2 = strtok(NULL, " ");
                }
                return;
            }
        } else if (escolha == 2) {
            printf("\n");
            while (true){
                printf("Primeira letra do intervalo: ");
                fgets(s, MAX_PAL * MB_CUR_MAX * sizeof(char), stdin);
                if (strlen(s) == MAX_PAL * MB_CUR_MAX * sizeof(char)-1){
                    flush_in();
                }
                if (strtok(s, " \n\r\t\v") == NULL){
                    printf("%sExprimente escrever alguma coisa!%s\n", C_BLUE, NONE);
                } else if (strtok(NULL, " \n\r\t\v") != NULL){
                    printf("%sNao insira mais do que uma letra!%s\n", C_RED, NONE);
                } else{
                    strtobase_u8(s1, s);
                    if (strlen(s1) != 1) {
                        printf("%sComando Inválido! Insira apenas uma letra!%s\n\n",C_RED,NONE);
                    } else{
                        bool verifica = false;
                        for (int i = 0; i < (int)strlen(dicio); ++i) {
                            if (dicio[i] == s1[0]){
                                verifica = true;
                                ind_ini = i;
                                break;
                            }
                        }
                        if (verifica == false){
                            printf("%sCaracter Invalido!%s\n\n",C_RED,NONE);
                        } else
                            break;
                    }
                }
            }
            while (true){
                printf("Ultima letra do intervalo: ");
                fgets(s, MAX_PAL * MB_CUR_MAX * sizeof(char), stdin);
                if (strlen(s) == MAX_PAL * MB_CUR_MAX * sizeof(char)-1){
                    flush_in();
                }
                if (strtok(s, " \n\r\t\v") == NULL){
                    printf("%sExprimente escrever alguma coisa!%s\n", C_BLUE, NONE);
                } else if (strtok(NULL, " \n\r\t\v") != NULL){
                    printf("%sNao insira mais do que uma letra!%s\n", C_RED, NONE);
                } else {
                    strtobase_u8(s1, s);
                    if (strlen(s1) != 1) {
                        printf("%sComando Inválido! Insira apenas uma letra!%s\n\n", C_RED, NONE);
                    } else {
                        bool verifica = false;
                        for (int i = 0; i < (int) strlen(dicio); ++i) {
                            if (dicio[i] == s1[0]) {
                                verifica = true;
                                ind_fin = i;
                                break;
                            }
                        }
                        if (verifica == false) {
                            printf("%sCaracter Invalido!%s\n\n", C_RED, NONE);
                        } else
                            break;
                    }
                }
            }
            if (ind_ini>ind_fin) {
                printf("%sA primeira letra tem de ser menor ou igual à última letra!%s\n", C_RED, NONE);
            } else{
                for (int i = ind_ini; i <= ind_fin; ++i) {
                    c[0] = dicio[i];
                    c[1] = '\0';
                    auxiliar = 0;
                    printf("\nLista de palavras com as iniciais <%c>:\n", c[0]);
                    pesquisa_palavras_arvore(ab->raiz, c, &auxiliar);
                    if (auxiliar == 0) {
                        printf("\t<Lista vazia>\n");
                    }
                }
                return;
            }
        } else if (escolha == 0) {
            return;
        } else {
            printf("%sComando inválido!%s\n", C_RED, NONE);
        }
    }
}

static void pesquisa_palavras_arvore(struct no *pn, char *pal_pesq, int *auxiliar) {
    if (pn == NULL)
        return;

    char pal1[MAX_PAL * MB_CUR_MAX * sizeof(char) + 1];
    strtobase_u8(pal1, pn->p.pal);
    bool ver = true;
    for (int i = 0; i < (int) strlen(pal_pesq); ++i) {
        if (pal1[i] != pal_pesq[i]) {
            ver = false;
        }
    }

    if (ver == true) {
        *auxiliar = 1;
        printf("\t%s\n", pn->p.pal);
        pesquisa_palavras_arvore(pn->left, pal_pesq, auxiliar);
        pesquisa_palavras_arvore(pn->right, pal_pesq, auxiliar);
    } else if (strcmp(pal1, pal_pesq) < 0) {
        pesquisa_palavras_arvore(pn->right, pal_pesq, auxiliar);
    } else {
        pesquisa_palavras_arvore(pn->left, pal_pesq, auxiliar);
    }
}

void flush_in() {
    int ch;
    do {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}