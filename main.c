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


No *busca_com_pai(No *arvore, unsigned chave, No **pai) {
    No *anterior = NULL;
    No *atual = arvore->esq;  // Começa pela raiz (arvore->esq)
    int w = -1;

    while (atual->bit > w) {
        anterior = atual;
        w = atual->bit;
        if (bit(chave, atual->bit) == 0)
            atual = atual->esq;
        else
            atual = atual->dir;
    }

    *pai = anterior;  // Retorna o nó pai
    return atual;     // Retorna o nó encontrado
}







void deleta(No **arvore, unsigned chave) {
    No *pai = NULL;
    No *t = busca_com_pai(*arvore, chave, &pai);  // Busca o nó e seu pai

    if (t == NULL || t->chave != chave) {
        printf("Chave %u não encontrada na árvore.\n", chave);
        return;  // Não encontrou a chave
    }

    // Se o nó encontrado é o único nó na árvore
    if (t == *arvore) {
        printf("Chave %u removida. A árvore agora está vazia.\n", chave);
        inicializa(arvore);  // Reinicializa a árvore
        return;
    }

    // Achar o nó substituto para eliminar t
    No *substituto = NULL;
    if (bit(t->chave, pai->bit) == 0)
        substituto = pai->dir;  // O substituto será o filho que não estamos removendo
    else
        substituto = pai->esq;

    // Conecta o pai ao substituto
    if (pai->esq == t)
        pai->esq = substituto;
    else
        pai->dir = substituto;

    // Libera o nó deletado
    free(t);

    printf("Chave %u removida da árvore.\n", chave);
}


// Função auxiliar para converter um número inteiro para uma string binária
void int_para_binario(unsigned n, char *bin_str) {
    int i;
    for (i = sizeof(unsigned) * 8 - 1; i >= 0; i--) {
        bin_str[sizeof(unsigned) * 8 - 1 - i] = (n & (1u << i)) ? '1' : '0';
    }
    bin_str[sizeof(unsigned) * 8] = '\0'; // Adiciona o terminador de string
}

// Função para imprimir a árvore (em pré-ordem) para facilitar o teste
void imprime_arvore(No *arvore) {
    if (arvore != NULL && arvore->chave != UINT_MAX) {
        char bin_str[sizeof(unsigned) * 8 + 1];  // Buffer para a string binária
        int_para_binario(arvore->chave, bin_str); // Converte a chave para binário
        printf("Chave: %u (%s), Bit: %d\n", arvore->chave, bin_str, arvore->bit);
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
        printf("4. Deletar um número\n");
        printf("5. Sair\n");
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
                printf("Digite o número a ser deletado: ");
                scanf("%u", &numero);
                deleta(&arvore, numero);
                break;

            case 5:
                printf("Saindo...\n");
                return 0;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}
