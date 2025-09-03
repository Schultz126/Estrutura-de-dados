#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 100
#define REGISTROS 5

// Estrutura para indexação
typedef struct {
    char chave[20];
    long posicao;
} Indice;

// Função de hash simples
int hash(const char *chave) {
    int h = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        h = (h * 31 + chave[i]) % TAM;
    return h;
}

// Cria arquivo com registros simulados
void criarArquivo(const char *nomeArquivo, Indice tabelaIndice[]) {
    FILE *f = fopen(nomeArquivo, "w");
    if (!f) {
        perror("Erro ao criar arquivo");
        exit(1);
    }

    char chaves[REGISTROS][20] = {"cliente1", "cliente2", "cliente3", "cliente4", "cliente5"};
    for (int i = 0; i < REGISTROS; i++) {
        long pos = ftell(f);
        fprintf(f, "%s: dados do cliente %d\n", chaves[i], i + 1);
        strcpy(tabelaIndice[i].chave, chaves[i]);
        tabelaIndice[i].posicao = pos;
    }

    fclose(f);
}

// Busca no arquivo usando índice hash
void buscarNoArquivo(const char *nomeArquivo, Indice tabelaIndice[], const char *chave) {
    for (int i = 0; i < REGISTROS; i++) {
        if (strcmp(tabelaIndice[i].chave, chave) == 0) {
            FILE *f = fopen(nomeArquivo, "r");
            if (!f) {
                perror("Erro ao abrir arquivo");
                exit(1);
            }
            fseek(f, tabelaIndice[i].posicao, SEEK_SET);
            char buffer[100];
            fgets(buffer, sizeof(buffer), f);
            printf("Registro encontrado: %s", buffer);
            fclose(f);
            return;
        }
    }

    printf("Registro com chave '%s' não encontrado.\n", chave);
}

int main() {
    const char *nomeArquivo = "dados.txt";
    Indice tabelaIndice[REGISTROS];

    criarArquivo(nomeArquivo, tabelaIndice);

    char chaveBusca[20];
    printf("Digite a chave para busca (ex: cliente3): ");
    scanf("%s", chaveBusca);

    buscarNoArquivo(nomeArquivo, tabelaIndice, chaveBusca);

    return 0;
}