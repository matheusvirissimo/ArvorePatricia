#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <limits.h> // usado para o UINT_MAX
#include <stdbool.h>

#define bits_na_chave 32 // quantidade de bits disponíveis por chave

typedef struct noPatricia{
    int bit; // posição do bit que discrimina o nó, isso é, para qual lado ele vai seguir 
    char *chave; // chave armazenada no nó. Será NULL para nós intermediários
    struct noPatricia *esq; // subárvore da esquerda 
    struct noPatricia *dir; // subárvore da direita
} NOPATRICIA; 

void inicializaArvore(NOPATRICIA **arvore){
    *arvore = (NOPATRICIA*) malloc(sizeof(NOPATRICIA));
    (*arvore)->chave = UINT_MAX; /* Maximum value an `unsigned int' can hold.  (Minimum is 0).  */
    (*arvore)->esq = (*arvore)->dir = *arvore;
    (*arvore)->bit = -1;
}

// Função para retornar o valor 0 ou 1 da chave sem considerar sinal (unsigned) do bit na posição k
unsigned bit(unsigned chave, int k){
    // chave >> é uma operação de deslocamento de bit à direta, que move até a posição k
    return chave >> (bits_na_chave - 1 - k) & 1;
    // faz uma operação de AND para saber qual é o bit.
}

// Inserção - inserir uma nova chave na árvore
NOPATRICIA* insereArvorePatricia(NOPATRICIA *arvore, unsigned chave, int w, NOPATRICIA *pai){
    NOPATRICIA *novo;

    if((arvore->bit) >= w || (arvore->bit <= pai->bit)){
        novo = (NOPATRICIA*) malloc(sizeof(NOPATRICIA));
        novo->chave = chave;
        novo->bit = w;

        if(bit(chave, novo->bit) == 1){ // bit a ser analisado é 1, ou seja, vai para a direita
            novo->esq = arvore; // o lado direito vai apontar para o resto da árvore
            novo->dir = novo; // o lado direito vai apontar para si mesmo, meio que em "loop"
        }else{ // o bit é 0
            novo->esq = novo; // vai apontar para si mesmo
            novo->dir = arvore; // aponta para o resto da árvore
        }

        return novo;
    }

    if(bit(chave, arvore->bit) == 0){
        arvore->esq = insereArvorePatricia(arvore->esq, chave, w, arvore);
    }else{
        arvore->dir = insereArvorePatricia(arvore->dir, chave, w, arvore);
    }
    
    return arvore;
}

// Remoção - Remover uma chave existente da árvore

// Busca - Verificar se uma chave está presente
NOPATRICIA* buscaRecursiva(NOPATRICIA *arvore, unsigned x, int w){
    // quando, na busca, o nó retrocede, ele faz a parada
    if(arvore->bit <= w){ // se ele é menor que o número passado como parâmetro, ele voltou
        return arvore; // encontrou o valor retorna o nó
    }

    if(bit(x, arvore->bit) == 0){ // se o bit analisado é 0, ele vai para a esquerda e passa para o próximo bit a ser analisado
        return buscaRecursiva(arvore->esq, x, arvore->bit);
    }else{ // do contrário, ele é 1, ou seja, vai para a esquerda
        return buscaRecursiva(arvore->dir, x, arvore->bit);
    }
}

NOPATRICIA* busca(NOPATRICIA *arvore, unsigned x){
    NOPATRICIA *t = buscaRecursiva(arvore->esq, x, -1); // passa o -1 porque inicia no nó dummy, o nó "inexistente"
    
    if(t->chave == x){ // se a busca retornar um nó, significa que ele encontrou o valor
        return t;
    }else{ // não encontrou nada. retorna null.
        return NULL;
    }
}
// Impressão - imprimir a árvore exibindo as chaves e a discriminação por bits