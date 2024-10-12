#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição do nó da árvore Patricia
typedef struct No {
    char *chave;  // A chave agora é uma string binária
    int bit; // posição do bit que discrimina o nó, isso é, para qual lado ele vai seguir
    struct No *esq; // nó da esquerda - quando o bit for 1
    struct No *dir; // nó da direita - quando o bit for 0
} NOPATRICIA;

// Função para inicializar a árvore
void inicializa(NOPATRICIA **arvore){

    printf("Inicializando a arvore...\n"); // Verifica se a inicialização ocorre
    *arvore = malloc(sizeof(NOPATRICIA)); // vamos criar um nó dummy = ele tem um valor que não é possível alcançar

    if(*arvore == NULL){ // deu algum problema e o nó não existe (?)
        printf("Falha ao alocar memoria para a arvore.\n");
        exit(1);  // Sai do programa se não puder alocar memória - mesma coisa que um return
    }
    (*arvore)->chave = NULL; // tem que ser nulo e não mais UNIT_MAX pois estamos trabalhando com char agora
    (*arvore)->esq = (*arvore)->dir = *arvore; // o nó aponta para sí mesmo tanto na esquerda quanto na direita
    (*arvore)->bit = -1; // como é a posição que discrimina o nó

    printf("Arvore inicializada com sucesso.\n"); // criamos o nó dummy
}

// Função auxiliar para obter o valor de um bit em uma string binária
int bit(const char *chave, int n){
    if (n >= strlen(chave)) {
        return 0;  // Se [n] ultrapassar o tamanho da string, considera 0
    }

    // Se o bit na posição [n] da chave for 1, retorna 1, do contrário, retorna 0
    return chave[n] == '1' ? 1 : 0; 
}

// Função de busca recursiva
NOPATRICIA *busca_rec(NOPATRICIA *arvore, const char *x, int w) {
    // recebe o nó da árvore, a palavra "x" e a posição w do bit

    // Se nós voltarmos na árvore, encontramos a chave
    if(arvore->bit <= w){
        return arvore;
    }


    if (bit(x, arvore->bit) == 0){
        // Se o bit nessa posição for 0, vamos avançar para a subárvore da ESQUERDA (BIT 0);
        return busca_rec(arvore->esq, x, arvore->bit);
    }else{
        // Se o bit nessa posição for 0, vamos avançar para a subárvore da DIREITA (BIT 1);
        return busca_rec(arvore->dir, x, arvore->bit);
    }
}

// Função de busca - passamos a árvore como um todo e o que estamos buscando.
NOPATRICIA *busca(NOPATRICIA *arvore, const char *x) {

    // (não entendi)
    if (arvore == NULL) {
        printf("Erro: Ponteiro da arvore é NULL em busca()\n");
        return NULL;
    }
    if (arvore->chave == NULL) {
        printf("Chave da arvore é NULL durante busca().\n");
        return NULL;  // Se a chave no nó atual for NULL, não encontramos a chave
    }
    
    //atribuimos a um novo nó "t" que vai nos auxiliar a fazer a busca.
    NOPATRICIA *t = busca_rec(arvore->esq, x, -1); // passamos a posição -1 porque começamos do dummy e vamos descendo

    // aqui, o "t" vai ter recebido o resultado da busca, então "perguntamos" se ele encontrou o char ou não

    // Se a chave dentro do nó for diferente de nulo (não encontrou) e o char que está sendo passado como parâmetro da função busca for 0, retornamos a árvore.
    return (t->chave != NULL && strcmp(t->chave, x) == 0) ? t : NULL;
}

// Função de inserção recursiva - passa a árvore, a nova chave a ser adicionada, a posição que vai ser discriminada "w" e o pai desse nó
NOPATRICIA *insere_rec(NOPATRICIA *arvore, const char *chave, int w, NOPATRICIA *pai){

    // vamos alocar memória para um novo nó
    NOPATRICIA *novo;

    if((arvore->bit >= w) || (arvore->bit <= pai->bit)){
        novo = malloc(sizeof(NOPATRICIA));
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
void insere(NOPATRICIA **arvore, const char *chave) {
    if (*arvore == NULL) {
        printf("Erro: Ponteiro da árvore é NULL em insere().\n");
        return;
    }

    NOPATRICIA *t = busca(*arvore, chave);
    if (t == NULL) {
        int w = 0;
        printf("Buscando ponto de inserção...\n");
        
        // Verifique se a chave no nó atual é NULL antes de fazer a comparação
        if ((*arvore)->chave != NULL) {
            while (bit(chave, w) == bit((*arvore)->chave, w)){
                
                printf("Teste"); // encontrado o problema
                w++;
            }
        }

        *arvore = insere_rec(*arvore, chave, w, *arvore);
    }
    printf("Chave %s inserida na árvore.\n", chave);
}


// Função para imprimir a árvore (em pré-ordem)
void imprime_arvore(NOPATRICIA *arvore) {
    if (arvore != NULL && arvore->chave != NULL) {
        printf("Chave: %s, Bit: %d\n", arvore->chave, arvore->bit);
        if (arvore->esq != arvore)
            imprime_arvore(arvore->esq);
        if (arvore->dir != arvore)
            imprime_arvore(arvore->dir);
    }
}

int main() {
    NOPATRICIA *arvore;
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
