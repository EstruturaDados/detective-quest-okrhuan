#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];           // Nome do cômodo
    struct Sala *esquerda;   // Caminho à esquerda
    struct Sala *direita;    // Caminho à direita
} Sala;

Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

void explorarSalas(Sala *atual) {
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Caso a sala não tenha caminhos
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Este é um cômodo sem saídas. A exploração terminou!\n");
            break;
        }

        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) printf("  [e] Ir para %s (esquerda)\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("  [d] Ir para %s (direita)\n", atual->direita->nome);
        printf("  [s] Sair do jogo\n> ");

        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else if (escolha == 's') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

int main() {
    // Criação manual das salas (árvore binária fixa)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");

    // Conexões (montagem da árvore)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    // Cozinha não tem filhos (folha)

    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão e descubra seus segredos...\n");

    explorarSalas(hall);

    // Liberação da memória (boa prática)
    free(biblioteca);
    free(jardim);
    free(salaEstar);
    free(cozinha);
    free(hall);

    return 0;
}
