#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];            
    char pista[100];          
    struct Sala *esquerda;    
    struct Sala *direita;     
} Sala;

// Estrutura da árvore BST para armazenar as pistas coletadas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;


Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

PistaNode* criarNoPista(const char *pista) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo == NULL) {
        printf("Erro ao alocar memória para pista.\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}


PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) return criarNoPista(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

void explorarSalasComPistas(Sala *atual, PistaNode **raizPistas) {
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Coleta da pista, se existir
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: \"%s\"\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        } else {
            printf("Nenhuma pista encontrada neste cômodo.\n");
        }

        printf("\nEscolha um caminho:\n");
        if (atual->esquerda != NULL) printf("  [e] Ir para %s (esquerda)\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("  [d] Ir para %s (direita)\n", atual->direita->nome);
        printf("  [s] Sair da exploração e ver pistas\n> ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else if (escolha == 's') {
            printf("\nEncerrando exploração...\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}


void liberarMemoriaSalas(Sala *raiz) {
    if (raiz != NULL) {
        liberarMemoriaSalas(raiz->esquerda);
        liberarMemoriaSalas(raiz->direita);
        free(raiz);
    }
}

void liberarMemoriaPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarMemoriaPistas(raiz->esquerda);
        liberarMemoriaPistas(raiz->direita);
        free(raiz);
    }
}


int main() {
    // ======= Criação manual da mansão =======
    Sala *hall = criarSala("Hall de Entrada", "Pegadas de sapato molhado");
    Sala *salaEstar = criarSala("Sala de Estar", "Relógio parado às 22h15");
    Sala *cozinha = criarSala("Cozinha", "Copo quebrado sobre a pia");
    Sala *biblioteca = criarSala("Biblioteca", "Livro faltando na prateleira");
    Sala *jardim = criarSala("Jardim", "");

    // Conexões entre as salas (árvore binária)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    // ======= Árvore de pistas (BST) =======
    PistaNode *raizPistas = NULL;

    printf("🔍 Bem-vindo ao Detective Quest: Coleta de Pistas!\n");
    printf("Explore a mansão e descubra os indícios do mistério...\n");

    // ======= Início da exploração =======
    explorarSalasComPistas(hall, &raizPistas);

    // ======= Exibe pistas coletadas =======
    printf("\n===== PISTAS COLETADAS =====\n");
    if (raizPistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(raizPistas);

    // ======= Liberação de memória =======
    liberarMemoriaSalas(hall);
    liberarMemoriaPistas(raizPistas);

    printf("\nFim da exploração. Até a próxima, detetive!\n");
    return 0;
}
