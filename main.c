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

