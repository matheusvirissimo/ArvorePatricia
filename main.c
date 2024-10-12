#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição do nó da árvore Patricia
typedef struct No {
    char *chave;  // A chave agora é uma string binaria
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

    // Se a posição do bit do nó atual for igual ou maior a posição passada 
    // OU se o bit do pai for maior ou igual ao da posição passada pelo novo nó
    if((arvore->bit >= w) || (arvore->bit <= pai->bit)){
        
        // Aloca memória do novo nó
        novo = malloc(sizeof(NOPATRICIA));
        if(novo == NULL){ // deu algum erro na alocação de memória no nó em si
            printf("Falha ao alocar memoria para o novo no.\n");
            exit(1);
        }

        novo->chave = malloc(strlen(chave) + 1);  // Aloca espaço para a nova chave e adiciona mais um por causa do '\0' em caso de string

        if (novo->chave == NULL){ // deu algum erro na hora de alocar memória para a chave
            printf("Falha ao alocar memoria para a chave.\n");
            exit(1);
        }

        // pode ser feito só um if para verificação de erros na alocação de memória, mas assim melhoramos a depuração de erros.

        strcpy(novo->chave, chave);  // Copia o valor da chave que passamos como parâmetro da função para o novo nó.

        novo->bit = w; // o bit desse nó vai se discriminar na posição w;

        if (bit(chave, novo->bit) == 1){ // se a chave na posição w (novo->bit) for igual a 1, ela vai ter filhos a esquerda
            novo->esq = arvore;
            novo->dir = novo; // aponta para si mesmo porque não tem nada parecido (a não ser ele mesmo) com ele até esse bit com o valor 0
        } else { // tem filhos a direita
            novo->esq = novo; // aponta para si mesmo
            novo->dir = arvore; // tem filhos a direita
        }
        return novo; // novo nó criado
    }
    if (bit(chave, arvore->bit) == 0){
        arvore->esq = insere_rec(arvore->esq, chave, w, arvore);
    }else{
        arvore->dir = insere_rec(arvore->dir, chave, w, arvore);
    }

    return arvore; // esse return é para caso o valor JÁ esteja inserido na árvore
}

// Função de inserção - ponteiro de ponteiro da árvore e a chave que vai ser inserida
void insere(NOPATRICIA **arvore, const char *chave) {

    // (não entendi)
    if (*arvore == NULL){
        printf("Erro: Ponteiro da arvore e NULL em insere().\n");
        return;
    }

    // 1°: procura pelo valor inserido 
    NOPATRICIA *t = busca(*arvore, chave);



    // 2°: encontrar o valor que tenha a maior quantidade de prefixos iguais, isso é, onde discrimina o bit
    if(t == NULL){
        int w = 0;
        printf("Buscando ponto de insercao...\n"); 
        
        
        // Verifique se a chave no nó atual é NULL antes de fazer a comparação
        if ((*arvore)->chave != NULL){ // enquanto a chave do nó não for nulo, 

            // 3°: faz uma nova busca na árvore verificando onde dividir ela
            while (bit(chave, w) == bit((*arvore)->chave, w)){
                
                printf("Teste"); // encontrado o problema
                w++;
            }
        }

        // vamos inserir na posição w onde discrimina o bit.
        *arvore = insere_rec(*arvore, chave, w, *arvore);
    }
    printf("Chave %s inserida na árvore.\n", chave);
}


// Função para imprimir a árvore (em pré-ordem)
// e se fizermos in-order?

// pré ordem: 1 - visita a raiz, 2 - percorre a subárvore da esq, 3 - percorre a subárvore da direita
void imprime_arvore(NOPATRICIA *arvore) {
    if (arvore != NULL && arvore->chave != NULL) {
        printf("Chave: %s, Bit: %d\n", arvore->chave, arvore->bit);
        if (arvore->esq != arvore){ // percorre a subárvore da esquerda
            imprime_arvore(arvore->esq);
        }
        if (arvore->dir != arvore){ // percorre a subárvore da direita
            imprime_arvore(arvore->dir);
        }
    }
}

int main() {
    NOPATRICIA *arvore;
    inicializa(&arvore);

    int opcao;
    char chave[100];

    while (1) {
        printf("\nEscolha uma opcao:\n");
        printf("1. Inserir uma chave binaria\n");
        printf("2. Buscar uma chave binaria\n");
        printf("3. Imprimir a arvore\n");
        printf("4. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite a chave binaria a ser inserida: ");
                scanf("%s", chave);
                printf("Inserindo chave: %s\n", chave);  // Para ver se a chave está correta

                insere(&arvore, chave);
                break;

            case 2:
                printf("Digite a chave binaria a ser buscada: ");
                scanf("%s", chave);
                if (busca(arvore, chave) != NULL)
                    printf("Chave %s encontrada na arvore.\n", chave);
                else
                    printf("Chave %s nao encontrada.\n", chave);
                break;

            case 3:
                printf("Arvore Patricia:\n");
                imprime_arvore(arvore);
                printf("\n\n");
                break;

            case 4:
                printf("Saindo...\n");
                return 0;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    }

    return 0;
}
