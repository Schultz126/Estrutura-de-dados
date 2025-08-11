#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100 // Tamanho inicial da tabela hash

// Definimos as estruturas com ponteiros
typedef struct {
    char cpf[6];
    char nome[30];
} CIDADE_DADOS;

// Estrutura para a tabela hash com lista encadeada
typedef struct TabHash {
    int pos;
    struct TabHash *link; // Ponteiro para o próximo nó da lista encadeada
} TABELA_HASH;

// Ponteiro para o array de dados da cidade
CIDADE_DADOS *cidade_dados = NULL;

// Ponteiro para o array de "cabeças" da tabela hash
TABELA_HASH **tabela = NULL;

int contarlinhas(char *arq) {
    FILE *fp = fopen(arq, "r");
    int ch = 0, linhas = 0;
    if (fp == NULL) return 0;
    linhas++;
    while ((ch = fgetc(fp)) != EOF)
        if (ch == '\n') linhas++;
    fclose(fp);
    return linhas;
}

int hash(char *chave) {
    return atoi(chave) % N;
}

// Insere um elemento na tabela hash
void inserirHash(char *chave, int pos) {
    int idx = hash(chave);

    // Aloca o novo nó da lista encadeada
    TABELA_HASH *novoNo = (TABELA_HASH*)malloc(sizeof(TABELA_HASH));
    if (novoNo == NULL) {
        printf("Erro de alocacao de memoria para o no da hash.\n");
        return;
    }
    novoNo->pos = pos;
    novoNo->link = NULL;

    // Se não há colisão, o novo nó se torna a cabeça da lista
    if (tabela[idx] == NULL) {
        tabela[idx] = novoNo;
    } else {
        // Se há colisão, percorre a lista até o final e insere
        TABELA_HASH *temp = tabela[idx];
        while (temp->link != NULL) {
            temp = temp->link;
        }
        temp->link = novoNo;
    }
}

// Busca uma chave na tabela hash
int buscaHash(char *chave, int *comp) {
    int idx = hash(chave);
    *comp = 0;

    // Percorre a lista encadeada no índice da hash
    TABELA_HASH *temp = tabela[idx];
    while (temp != NULL) {
        ++*comp;
        if (strcmp(cidade_dados[temp->pos].cpf, chave) == 0) {
            return temp->pos;
        }
        temp = temp->link;
    }
    return -1; // Não encontrado
}

void construirHash() {
    // Aloca a tabela de hash
    tabela = (TABELA_HASH**)malloc(N * sizeof(TABELA_HASH*));
    if (tabela == NULL) {
        printf("Erro de alocacao de memoria para a tabela hash.\n");
        return;
    }
    // Inicializa todos os ponteiros da tabela como NULL
    for (int i = 0; i < N; ++i) {
        tabela[i] = NULL;
    }

    for (int i = 0; i < N; ++i) {
        inserirHash(cidade_dados[i].cpf, i);
    }
}

void imprimirTabela() {
    printf("\nTabela Hash:\n");
    for (int i = 0; i < N; i++) {
        printf("[%d]", i);
        TABELA_HASH *temp = tabela[i];
        while (temp != NULL) {
            printf(" -> Chave: %s, Valor: %s", cidade_dados[temp->pos].cpf, cidade_dados[temp->pos].nome);
            temp = temp->link;
        }
        printf("\n");
    }
}

// Libera toda a memória alocada dinamicamente
void liberarMemoria() {
    // Libera a memória de cidade_dados
    if (cidade_dados != NULL) {
        free(cidade_dados);
        cidade_dados = NULL;
    }

    // Libera a memória da tabela hash
    if (tabela != NULL) {
        for (int i = 0; i < N; ++i) {
            TABELA_HASH *temp = tabela[i];
            while (temp != NULL) {
                TABELA_HASH *proximo = temp->link;
                free(temp);
                temp = proximo;
            }
        }
        free(tabela);
        tabela = NULL;
    }
}

int main(int argc, char *argv[]) {
    // Aloca a memória para os dados
    cidade_dados = (CIDADE_DADOS*)malloc(N * sizeof(CIDADE_DADOS));
    if (cidade_dados == NULL) {
        printf("Erro de alocacao de memoria para os dados da cidade.\n");
        return 1;
    }

    // O resto da sua lógica original para carregar os dados...
    int cpf_ant = 1005;
    FILE *fn = fopen("nomes.txt", "rt");
    if (fn == NULL) {
        printf("ERRO: arquivo 'nomes.txt' nao encontrado.\n");
        liberarMemoria();
        return 1;
    }
    for (int i = 0; i < N; i++) {
        sprintf(cidade_dados[i].cpf, "%05d", (cpf_ant += rand() % 100 + 1));
        fscanf(fn, "%s", cidade_dados[i].nome);
        printf("%d: %s => %s\n", i, cidade_dados[i].cpf, cidade_dados[i].nome);
    }
    fclose(fn);

    construirHash();
    imprimirTabela();

    int busca_chave_idx = rand() % N;
    int compar = 0;
    int achou = buscaHash(cidade_dados[busca_chave_idx].cpf, &compar);

    if (achou != -1) {
        printf("HASH com encadeamento: %d comparacoes => achou %d chave %s\n", compar, achou, cidade_dados[busca_chave_idx].cpf);
    } else {
        printf("HASH com encadeamento: %d comparacoes => chave %s nao encontrada\n", compar, cidade_dados[busca_chave_idx].cpf);
    }

    liberarMemoria(); // Chama a função para liberar a memória alocada

    return 0;
}
