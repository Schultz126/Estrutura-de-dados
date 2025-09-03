#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 10

typedef struct Registro {
    char chave[20];
    long posicao;
    struct Registro* prox;
} Registro;

Registro* tabela[TAM];

// Função hash simples
int hash(const char* chave) {
    int h = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        h = (h * 31 + chave[i]) % TAM;
    return h;
}

// Inserção na tabela com encadeamento
void inserirIndice(const char* chave, long posicao) {
    int indice = hash(chave);
    Registro* novo = (Registro*)malloc(sizeof(Registro));
    strcpy(novo->chave, chave);
    novo->posicao = posicao;
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

// Criação do arquivo e preenchimento do índice
void criarArquivoComIndice(const char* nomeArquivo) {
    FILE* f = fopen(nomeArquivo, "w");
    if (!f) {
        perror("Erro ao criar arquivo");
        exit(1);
    }

    char chaves[][20] = {"cliente1", "cliente12", "cliente23", "cliente34", "cliente45"};
    for (int i = 0; i < 5; i++) {
        long pos = ftell(f);
        fprintf(f, "%s: dados do cliente %d\n", chaves[i], i + 1);
        inserirIndice(chaves[i], pos);
    }

    fclose(f);
}

// Busca com encadeamento
void buscarRegistro(const char* nomeArquivo, const char* chaveBusca) {
    int indice = hash(chaveBusca);
    Registro* atual = tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->chave, chaveBusca) == 0) {
            FILE* f = fopen(nomeArquivo, "r");
            if (!f) {
                perror("Erro ao abrir arquivo");
                exit(1);
            }
            fseek(f, atual->posicao, SEEK_SET);
            char buffer[100];
            fgets(buffer, sizeof(buffer), f);
            printf("Registro encontrado: %s", buffer);
            fclose(f);
            return;
        }
        atual = atual->prox;
    }

    printf("Registro com chave '%s' não encontrado.\n", chaveBusca);
}

// Liberação da memória
void liberarTabela() {
    for (int i = 0; i < TAM; i++) {
        Registro* atual = tabela[i];
        while (atual) {
            Registro* temp = atual;
            atual = atual->prox;
            free(temp);
        }
        tabela[i] = NULL;
    }
}

int main() {
    const char* nomeArquivo = "clientes_hash.txt";
    criarArquivoComIndice(nomeArquivo);

    char chaveBusca[20];
    printf("Digite a chave para busca (ex: cliente12): ");
    scanf("%s", chaveBusca);

    buscarRegistro(nomeArquivo, chaveBusca);
    liberarTabela();

    return 0;
}