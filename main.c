#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

