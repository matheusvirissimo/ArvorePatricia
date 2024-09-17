#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define BITS_NA_CHAVE 32

typedef struct noPatricia {
    int bit;
    unsigned int chave;
    struct noPatricia *esq;
    struct noPatricia *dir;
} NOPATRICIA;

// Função para criar um novo nó da árvore Patricia
NOPATRICIA* criaNo(unsigned int chave, int bit) {
    NOPATRICIA *novoNo = (NOPATRICIA*) malloc(sizeof(NOPATRICIA));
    
    if (novoNo == NULL) {
        printf("Erro: falha ao alocar memória para novo nó.\n");
        return NULL;
    }

    novoNo->chave = chave;
    novoNo->bit = bit;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    
    return novoNo;
}

void inicializaArvore(NOPATRICIA **arvore) {
    *arvore = NULL;
}

unsigned bit(unsigned chave, int k) {
    if (k < 0 || k >= BITS_NA_CHAVE) {
        return 0;
    }
    return (chave >> (BITS_NA_CHAVE - 1 - k)) & 1;
}

NOPATRICIA* insereArvorePatricia(NOPATRICIA *arvore, unsigned chave, int w, NOPATRICIA *pai) {
    if (arvore == NULL) {
        NOPATRICIA *novo = criaNo(chave, w);
        if (novo == NULL) {
            return NULL;
        }
        return novo;
    }

    if (arvore->bit >= w || arvore->bit <= pai->bit) {
        NOPATRICIA *novo = criaNo(chave, w);
        if (novo == NULL) {
            return NULL;
        }

        if (bit(chave, novo->bit) == 1) {
            novo->esq = arvore;
            novo->dir = NULL;
        } else {
            novo->esq = NULL;
            novo->dir = arvore;
        }

        return novo;
    }

    if (bit(chave, arvore->bit) == 0) {
        arvore->esq = insereArvorePatricia(arvore->esq, chave, w, arvore);
    } else {
        arvore->dir = insereArvorePatricia(arvore->dir, chave, w, arvore);
    }

    return arvore;
}

NOPATRICIA* buscaRecursiva(NOPATRICIA *arvore, unsigned x, int w) {
    if (arvore == NULL) {
        return NULL;
    }

    if (arvore->bit <= w) {
        return arvore;
    }

    if (bit(x, arvore->bit) == 0) {
        return buscaRecursiva(arvore->esq, x, arvore->bit);
    } else {
        return buscaRecursiva(arvore->dir, x, arvore->bit);
    }
}

NOPATRICIA* busca(NOPATRICIA *arvore, unsigned x) {
    NOPATRICIA *t = buscaRecursiva(arvore, x, -1);
    if (t != NULL && t->chave == x) {
        return t;
    }
    return NULL;
}

int main() {
    NOPATRICIA *arvore;
    inicializaArvore(&arvore);

    arvore = insereArvorePatricia(arvore, 10, 0, NULL);
    arvore = insereArvorePatricia(arvore, 15, 0, NULL);
    arvore = insereArvorePatricia(arvore, 7, 0, NULL);

    NOPATRICIA *resultado = busca(arvore, 10);
    if (resultado != NULL) {
        printf("Chave 10 encontrada na árvore!\n");
    } else {
        printf("Chave 10 não encontrada na árvore!\n");
    }

    resultado = busca(arvore, 15);
    if (resultado != NULL) {
        printf("Chave 15 encontrada na árvore!\n");
    } else {
        printf("Chave 15 não encontrada na árvore!\n");
    }

    resultado = busca(arvore, 7);
    if (resultado != NULL) {
        printf("Chave 7 encontrada na árvore!\n");
    } else {
        printf("Chave 7 não encontrada na árvore!\n");
    }

    resultado = busca(arvore, 20);
    if (resultado != NULL) {
        printf("Chave 20 encontrada na árvore!\n");
    } else {
        printf("Chave 20 não encontrada na árvore!\n");
    }

    return 0;
}
