#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição do nó da árvore Patricia
typedef struct No {
    char *chave;  // A chave agora é uma string binária
    int bit;
    struct No *esq, *dir;
} No;

// Função para inicializar a árvore
void inicializa(No **arvore) {
    printf("Inicializando a árvore...\n"); // Verifica se a inicialização ocorre
    *arvore = malloc(sizeof(No));
    if (*arvore == NULL) {
        printf("Falha ao alocar memória para a árvore.\n");
        exit(1);  // Sai do programa se não puder alocar memória
    }
    (*arvore)->chave = NULL;  // Chave NULL para o nó sentinela
    (*arvore)->esq = (*arvore)->dir = *arvore;
    (*arvore)->bit = -1;
    printf("Árvore inicializada com sucesso.\n");
}

// Função auxiliar para obter o valor de um bit em uma string binária
int bit(const char *chave, int n) {
    if (n >= strlen(chave)) {
        return 0;  // Se n ultrapassar o tamanho da string, considera 0
    }
    return chave[n] == '1' ? 1 : 0;
}

// Função de busca recursiva
No *busca_rec(No *arvore, const char *x, int w) {
    if (arvore->bit <= w)
        return arvore;
    if (bit(x, arvore->bit) == 0)
        return busca_rec(arvore->esq, x, arvore->bit);
    else
        return busca_rec(arvore->dir, x, arvore->bit);
}

// Função de busca pública
No *busca(No *arvore, const char *x) {
    if (arvore == NULL) {
        printf("Erro: Ponteiro da árvore é NULL em busca()\n");
        return NULL;
    }
    if (arvore->chave == NULL) {
        printf("Chave da árvore é NULL durante busca().\n");
        return NULL;  // Se a chave no nó atual for NULL, não encontramos a chave
    }
    
    No *t = busca_rec(arvore->esq, x, -1);
    return (t->chave != NULL && strcmp(t->chave, x) == 0) ? t : NULL;
}

// Função de inserção recursiva
No *insere_rec(No *arvore, const char *chave, int w, No *pai) {
    No *novo;
    if ((arvore->bit >= w) || (arvore->bit <= pai->bit)) {
        novo = malloc(sizeof(No));
        if (novo == NULL) {
            printf("Falha ao alocar memória para o novo nó.\n");
            exit(1);
        }
        novo->chave = malloc(strlen(chave) + 1);  // Aloca espaço para a nova chave
        if (novo->chave == NULL) {
            printf("Falha ao alocar memória para a chave.\n");
            exit(1);
        }
        strcpy(novo->chave, chave);  // Copia a chave
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
void insere(No **arvore, const char *chave) {
    if (*arvore == NULL) {
        printf("Erro: Ponteiro da árvore é NULL em insere().\n");
        return;
    }

    No *t = busca(*arvore, chave);
    if (t == NULL) {
        int w = 0;
        printf("Buscando ponto de inserção...\n");
        
        // Verifique se a chave no nó atual é NULL antes de fazer a comparação
        if ((*arvore)->chave != NULL) {
            while (bit(chave, w) == bit((*arvore)->chave, w))
                w++;
        }

        *arvore = insere_rec(*arvore, chave, w, *arvore);
    }
    printf("Chave %s inserida na árvore.\n", chave);
}


// Função para imprimir a árvore (em pré-ordem)
void imprime_arvore(No *arvore) {
    if (arvore != NULL && arvore->chave != NULL) {
        printf("Chave: %s, Bit: %d\n", arvore->chave, arvore->bit);
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
    char chave[100];

    while (1) {
        printf("\nEscolha uma opção:\n");
        printf("1. Inserir uma chave binária\n");
        printf("2. Buscar uma chave binária\n");
        printf("3. Imprimir a árvore\n");
        printf("4. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite a chave binária a ser inserida: ");
                scanf("%s", chave);
                printf("Inserindo chave: %s\n", chave);  // Para ver se a chave está correta

                insere(&arvore, chave);
                break;

            case 2:
                printf("Digite a chave binária a ser buscada: ");
                scanf("%s", chave);
                if (busca(arvore, chave) != NULL)
                    printf("Chave %s encontrada na árvore.\n", chave);
                else
                    printf("Chave %s não encontrada.\n", chave);
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
