#include <stdio.h>
#include <stdlib.h>

#define N 5
#define TAM (N * 2)
 
typedef struct {
    int chave;
    int valor;
    int ocupado;
} Registro;

Registro tabela[TAM];

// Função hash simples para inteiros
int hash(int chave) {
    return chave % N; // Usa apenas os primeiros N como área principal
}

// Inicializa a tabela
void inicializarTabela() {
    for (int i = 0; i < TAM; i++) {
        tabela[i].ocupado = 0;
    }
}

// Insere um par (chave, valor) na tabela
void inserir(int chave, int valor) {
    int idx = hash(chave);

    if (!tabela[idx].ocupado) {
        tabela[idx].chave = chave;
        tabela[idx].valor = valor;
        tabela[idx].ocupado = 1;
        printf("Inserido %d:%d na posicao %d (area principal)\n", chave, valor, idx);
    } else {
        // Sondagem linear na área de transbordo (N até 2N)
        for (int i = N; i < TAM; i++) {
            if (!tabela[i].ocupado) {
                tabela[i].chave = chave;
                tabela[i].valor = valor;
                tabela[i].ocupado = 1;
                printf("Inserido %d:%d na posicao %d (area reserva)\n", chave, valor, i);
                return;
            }
        }
        printf("Erro: tabela cheia, nao foi possivel inserir %d\n", chave);
    }
}

// Busca uma chave na tabela
int buscar(int chave) {
    int idx = hash(chave);

    if (tabela[idx].ocupado && tabela[idx].chave == chave) {
        return tabela[idx].valor;
    }

    for (int i = N; i < TAM; i++) {
        if (tabela[i].ocupado && tabela[i].chave == chave) {
            return tabela[i].valor;
        }
    }

    return -1; // não encontrado
}

void imprimirTabela() {
    printf("\nTabela Hash:\n");
    for (int i = 0; i < TAM; i++) {
        if (tabela[i].ocupado)
            printf("[%d] -> Chave: %d, Valor: %d\n", i, tabela[i].chave, tabela[i].valor);
        else
            printf("[%d] -> (vazio)\n", i);
    }
}

int main() {
    inicializarTabela();

    inserir(1, 100); 
    inserir(6, 200);
    inserir(11, 300);
    inserir(16, 400);
    inserir(21, 500);
    inserir(26, 600);
    inserir(31, 700);

    imprimirTabela();

    int chave = 11;
    int resultado = buscar(chave);
    if (resultado != -1)
        printf("\nBusca: Chave %d encontrada com valor %d\n", chave, resultado);
    else
        printf("\nBusca: Chave %d nao encontrada\n", chave);

    return 0;
}