#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Definição do nó da árvore Patricia
typedef struct No {
    unsigned chave;
    int bit;
    struct No *esq, *dir;
} No;

// Função auxiliar para obter o valor do bit em uma posição específica
int bit(unsigned x, int n) {
    return (x >> n) & 1;
}

// Função para inicializar a árvore
void inicializa(No **arvore) {
    *arvore = malloc(sizeof(No));
    (*arvore)->chave = UINT_MAX;
    (*arvore)->esq = (*arvore)->dir = *arvore;
    (*arvore)->bit = -1;
}

// Função de busca recursiva
No *busca_rec(No *arvore, unsigned x, int w) {
    if (arvore->bit <= w)
        return arvore;
    if (bit(x, arvore->bit) == 0)
        return busca_rec(arvore->esq, x, arvore->bit);
    else
        return busca_rec(arvore->dir, x, arvore->bit);
}

// Função de busca pública
No *busca(No *arvore, unsigned x) {
    No *t = busca_rec(arvore->esq, x, -1);
    return t->chave == x ? t : NULL;
}

// Função de inserção recursiva
No *insere_rec(No *arvore, unsigned chave, int w, No *pai) {
    No *novo;
    if ((arvore->bit >= w) || (arvore->bit <= pai->bit)) {
        novo = malloc(sizeof(No));
        novo->chave = chave;
        novo->bit = w;
        if (bit(chave, novo->bit) == 1) {
            novo->esq = arvore;
            novo->dir = novo;
        } else {
            novo->esq = novo;
            novo->dir = arvore;
        }
        return novo;
    }
    if (bit(chave, arvore->bit) == 0)
        arvore->esq = insere_rec(arvore->esq, chave, w, arvore);
    else
        arvore->dir = insere_rec(arvore->dir, chave, w, arvore);
    return arvore;
}

// Função de inserção pública
void insere(No **arvore, unsigned chave) {
    No *t = busca(*arvore, chave);
    if (t == NULL) {
        int w = 0;
        while (bit(chave, w) == bit((*arvore)->chave, w))
            w++;
        *arvore = insere_rec(*arvore, chave, w, *arvore);
    }
}

// Função para imprimir a árvore (em pré-ordem) para facilitar o teste
void imprime_arvore(No *arvore) {
    if (arvore != NULL && arvore->chave != UINT_MAX) {
        printf("Chave: %u, Bit: %d\n", arvore->chave, arvore->bit);
        if (arvore->esq != arvore)
            imprime_arvore(arvore->esq);
        if (arvore->dir != arvore)
            imprime_arvore(arvore->dir);
    }
}

int main() {
    No *arvore;
    inicializa(&arvore);

    int opcao;
    unsigned numero;

    while (1) {
        printf("\nEscolha uma opção:\n");
        printf("1. Inserir um número\n");
        printf("2. Buscar um número\n");
        printf("3. Imprimir a árvore\n");
        printf("4. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o número a ser inserido: ");
                scanf("%u", &numero);
                insere(&arvore, numero);
                printf("Número %u inserido na árvore.\n", numero);
                break;
            
            case 2:
                printf("Digite o número a ser buscado: ");
                scanf("%u", &numero);
                if (busca(arvore, numero) != NULL)
                    printf("Número %u encontrado na árvore.\n", numero);
                else
                    printf("Número %u não encontrado.\n", numero);
                break;

            case 3:
                printf("Árvore Patricia:\n");
                imprime_arvore(arvore);
                break;

            case 4:
                printf("Saindo...\n");
                return 0;
            
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}
