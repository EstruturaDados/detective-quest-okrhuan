#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME 64
#define MAX_PISTA 128
#define HASH_SIZE 101  // número primo simples para hash



typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA];     
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;


typedef struct PistaNode {
    char pista[MAX_PISTA];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;


typedef struct HashNode {
    char pista[MAX_PISTA];
    char suspeito[MAX_NOME];
    struct HashNode *next;
} HashNode;


typedef struct {
    HashNode *buckets[HASH_SIZE];
} HashTable;


Sala* criarSala(const char *nome, const char *pista);


void explorarSalas(Sala *atual, PistaNode **raizPistas, HashTable *ht);


PistaNode* inserirPista(PistaNode *raiz, const char *pista);


void adicionarPista(PistaNode **raiz, const char *pista);


void inserirNaHash(HashTable *ht, const char *pista, const char *suspeito);


const char* encontrarSuspeito(HashTable *ht, const char *pista);


void exibirPistas(PistaNode *raiz);


int contarPistasParaSuspeito(PistaNode *raiz, HashTable *ht, const char *suspeito);


void verificarSuspeitoFinal(PistaNode *raiz, HashTable *ht, const char *acusado);


void liberarSalas(Sala *raiz);
void liberarPistas(PistaNode *raiz);
void liberarHash(HashTable *ht);
unsigned long hashString(const char *str);
void trim_newline(char *s);
void limparEntradaRestante(void);


Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if (!s) {
        fprintf(stderr, "Erro: falha na alocação de memória para Sala.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(s->nome, nome, MAX_NOME - 1);
    s->nome[MAX_NOME - 1] = '\0';
    if (pista && strlen(pista) > 0)
        strncpy(s->pista, pista, MAX_PISTA - 1);
    else
        s->pista[0] = '\0';
    s->pista[MAX_PISTA - 1] = '\0';
    s->esquerda = s->direita = NULL;
    return s;
}

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        if (!novo) {
            fprintf(stderr, "Erro: falha na alocação de memória para PistaNode.\n");
            exit(EXIT_FAILURE);
        }
        strncpy(novo->pista, pista, MAX_PISTA - 1);
        novo->pista[MAX_PISTA - 1] = '\0';
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    } else {
        
    }
    return raiz;
}

void adicionarPista(PistaNode **raiz, const char *pista) {
    if (pista == NULL || strlen(pista) == 0) return;
    *raiz = inserirPista(*raiz, pista);
}

unsigned long hashString(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char) *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_SIZE;
}

void inserirNaHash(HashTable *ht, const char *pista, const char *suspeito) {
    if (!ht || !pista || !suspeito) return;
    unsigned long idx = hashString(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    if (!novo) {
        fprintf(stderr, "Erro: falha na alocação de memória para HashNode.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(novo->pista, pista, MAX_PISTA - 1);
    novo->pista[MAX_PISTA - 1] = '\0';
    strncpy(novo->suspeito, suspeito, MAX_NOME - 1);
    novo->suspeito[MAX_NOME - 1] = '\0';
    novo->next = ht->buckets[idx];
    ht->buckets[idx] = novo;
}

const char* encontrarSuspeito(HashTable *ht, const char *pista) {
    if (!ht || !pista) return NULL;
    unsigned long idx = hashString(pista);
    HashNode *cur = ht->buckets[idx];
    while (cur) {
        if (strcmp(cur->pista, pista) == 0)
            return cur->suspeito;
        cur = cur->next;
    }
    return NULL;
}

void explorarSalas(Sala *atual, PistaNode **raizPistas, HashTable *ht) {
    if (!atual) return;
    char escolha;
    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            adicionarPista(raizPistas, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("\nOpções:\n");
        if (atual->esquerda) printf("  [e] Ir para %s (esquerda)\n", atual->esquerda->nome);
        if (atual->direita)  printf("  [d] Ir para %s (direita)\n", atual->direita->nome);
        printf("  [s] Sair da exploração (ir para julgamento)\n> ");

        if (scanf(" %c", &escolha) != 1) {
            printf("Entrada inválida.\n");
            limparEntradaRestante();
            continue;
        }
        limparEntradaRestante();

        if (escolha == 'e') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Caminho à esquerda não disponível.\n");
        } else if (escolha == 'd') {
            if (atual->direita) atual = atual->direita;
            else printf("Caminho à direita não disponível.\n");
        } else if (escolha == 's') {
            printf("Saindo da exploração...\n");
            break;
        } else {
            printf("Opção inválida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

void exibirPistas(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

int contarPistasParaSuspeito(PistaNode *raiz, HashTable *ht, const char *suspeito) {
    if (!raiz) return 0;
    int count = 0;
  
    count += contarPistasParaSuspeito(raiz->esquerda, ht, suspeito);
    const char *s = encontrarSuspeito(ht, raiz->pista);
    if (s && strcmp(s, suspeito) == 0) count++;
    count += contarPistasParaSuspeito(raiz->direita, ht, suspeito);
    return count;
}

void verificarSuspeitoFinal(PistaNode *raiz, HashTable *ht, const char *acusado) {
    int qtd = contarPistasParaSuspeito(raiz, ht, acusado);
    printf("\nO suspeito acusado: %s\n", acusado);
    printf("Número de pistas coletadas que apontam para %s: %d\n", acusado, qtd);
    if (qtd >= 2) {
        printf("\nVeredicto: Há evidências suficientes. A acusação é sustentada.\n");
    } else {
        printf("\nVeredicto: Evidências insuficientes. A acusação não é sustentada.\n");
    }
}

void liberarSalas(Sala *raiz) {
    if (!raiz) return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}

void liberarPistas(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

void liberarHash(HashTable *ht) {
    if (!ht) return;
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashNode *cur = ht->buckets[i];
        while (cur) {
            HashNode *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        ht->buckets[i] = NULL;
    }
}

void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n == 0) return;
    if (s[n-1] == '\n') s[n-1] = '\0';
}

void limparEntradaRestante(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}


int main(void) {
  
    Sala *hall = criarSala("Hall de Entrada", "Pegadas de sapato molhado");
    Sala *salaEstar = criarSala("Sala de Estar", "Relógio parado às 22h15");
    Sala *cozinha = criarSala("Cozinha", "Copo quebrado sobre a pia");
    Sala *biblioteca = criarSala("Biblioteca", "Livro faltando na prateleira");
    Sala *escritorio = criarSala("Escritorio", "Bilhete com nome riscado");

  
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;
    
    HashTable ht;
    for (int i = 0; i < HASH_SIZE; ++i) ht.buckets[i] = NULL;

    
    inserirNaHash(&ht, "Pegadas de sapato molhado", "Sr. Almeida");
    inserirNaHash(&ht, "Relógio parado às 22h15", "Sra. Oliveira");
    inserirNaHash(&ht, "Copo quebrado sobre a pia", "Sr. Almeida");
    inserirNaHash(&ht, "Livro faltando na prateleira", "Professor Maia");
    inserirNaHash(&ht, "Bilhete com nome riscado", "Sra. Oliveira");

    
    PistaNode *raizPistas = NULL;

    printf("=== Detective Quest: Acusação Final ===\n");
    printf("Explore a mansão e colete pistas. Ao sair, acuse um suspeito.\n");

    
    explorarSalas(hall, &raizPistas, &ht);

  
    printf("\n===== PISTAS COLETADAS (ordem alfabética) =====\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista coletada.\n");
    } else {
        exibirPistas(raizPistas);
    }

    
    char acusado[MAX_NOME];
    printf("\nDigite o nome do suspeito que você deseja acusar: ");
    if (fgets(acusado, sizeof(acusado), stdin) == NULL) {
        printf("Entrada inválida. Encerrando.\n");
    } else {
        trim_newline(acusado);
        if (strlen(acusado) == 0) {
            printf("Nenhum suspeito inserido. Encerrando.\n");
        } else {
            verificarSuspeitoFinal(raizPistas, &ht, acusado);
        }
    }

    
    liberarSalas(hall);
    liberarPistas(raizPistas);
    liberarHash(&ht);

    printf("\nFim do jogo. Obrigado por jogar.\n");
    return 0;
}
