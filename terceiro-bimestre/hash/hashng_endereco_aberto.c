#include <stdio.h>
#include <stdlib.h>

// ===================== Configuração =====================
#define TABLE_SIZE 17  // prefira primo para dupla hashing/quadrática

typedef enum { EMPTY = 0, OCCUPIED = 1, DELETED = 2 } SlotState;

typedef struct {
    int key;
    int value;
    SlotState state;
} Entry;

typedef enum { LINEAR = 1, QUADRATIC = 2, DOUBLE_HASH = 3 } ProbeMethod;

// ===================== Tabela Hash =====================
typedef struct {
    Entry table[TABLE_SIZE];
    ProbeMethod method;
} HashTable;

// Funções hash base
static inline int hash1(int k) {
    if (k < 0) k = -k;
    return k % TABLE_SIZE;
}

// Segunda função para duplo hash (nunca deve retornar 0)
static inline int hash2(int k) {
    if (k < 0) k = -k;
    // escolha comum: p menor que TABLE_SIZE; usar (p - (k % p))
    // alternativa: 1 + (k % (TABLE_SIZE - 2)) garante passo em [1..TABLE_SIZE-2]
    return 1 + (k % (TABLE_SIZE - 2));
}

// Função de endereçamento aberto, conforme método
static inline int probe_index(HashTable* ht, int key, int i) {
    int h = hash1(key);
    switch (ht->method) {
        case LINEAR:
            // h(k,i) = [ h(k) + i ] mod n
            return (h + i) % TABLE_SIZE;
        case QUADRATIC:
            // h(k,i) = [ h(k) + i^2 ] mod n
            return (h + i * i) % TABLE_SIZE;
        case DOUBLE_HASH:
        default: {
            // h(k,i) = [ h(k) + i * h'(k) ] mod n
            int step = hash2(key);
            return (h + i * step) % TABLE_SIZE;
        }
    }
}

void ht_init(HashTable* ht, ProbeMethod method) {
    ht->method = method;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        ht->table[i].state = EMPTY;
        ht->table[i].key = 0;
        ht->table[i].value = 0;
    }
}

// Inserção com tratamento de tombstone (reuso de DELETED)
// Retorna 1 se inseriu/atualizou, 0 se tabela cheia
int ht_insert(HashTable* ht, int key, int value) {
    int first_deleted = -1;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int idx = probe_index(ht, key, i);
        Entry* e = &ht->table[idx];

        if (e->state == OCCUPIED && e->key == key) {
            // Atualiza valor se a chave já existe
            e->value = value;
            return 1;
        }

        if (e->state == DELETED && first_deleted == -1) {
            first_deleted = idx; // marca primeiro tombstone encontrado
        }

        if (e->state == EMPTY) {
            // Inserir aqui (ou no primeiro DELETED, se houver)
            int target = (first_deleted != -1) ? first_deleted : idx;
            ht->table[target].key = key;
            ht->table[target].value = value;
            ht->table[target].state = OCCUPIED;
            return 1;
        }
    }

    // Tabela cheia ou ciclo completo: se encontrou tombstone, insere nele
    if (first_deleted != -1) {
        ht->table[first_deleted].key = key;
        ht->table[first_deleted].value = value;
        ht->table[first_deleted].state = OCCUPIED;
        return 1;
    }
    return 0;
}

// Busca: retorna 1 e preenche *out_value se encontrou; 0 caso contrário
int ht_search(HashTable* ht, int key, int* out_value) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int idx = probe_index(ht, key, i);
        Entry* e = &ht->table[idx];

        if (e->state == EMPTY) {
            // Em endereçamento aberto, EMPTY significa que a chave não está presente
            return 0;
        }
        if (e->state == OCCUPIED && e->key == key) {
            if (out_value) *out_value = e->value;
            return 1;
        }
        // Se DELETED ou OCCUPIED com outra chave, continua sondando
    }
    return 0; // não achou após varrer a tabela
}

// Remoção: marca como DELETED. Retorna 1 se removeu, 0 se não encontrou.
int ht_remove(HashTable* ht, int key) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int idx = probe_index(ht, key, i);
        Entry* e = &ht->table[idx];
        if (e->state == EMPTY) {
            return 0;
        }
        if (e->state == OCCUPIED && e->key == key) {
            e->state = DELETED;
            return 1;
        }
    }
    return 0;
}

void ht_print(HashTable* ht) {
    printf("\n===== Tabela (método %d) =====\n", ht->method);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Entry* e = &ht->table[i];
        printf("[%2d] ", i);
        if (e->state == EMPTY)      printf("EMPTY\n");
        else if (e->state == DELETED) printf("DELETED\n");
        else                        printf("key=%d val=%d\n", e->key, e->value);
    }
}

// ===================== Demonstração =====================
void demo(ProbeMethod m) {
    HashTable ht;
    ht_init(&ht, m);

    int keys[]   = {18, 35, 52, 69, 86, 103, 120, 137};
    int values[] = {180,350,520,690,860, 1030,1200,1370};
    int n = sizeof(keys)/sizeof(keys[0]);

    printf("\n--- Inserindo (método %d) ---\n", m);
    for (int i = 0; i < n; ++i) {
        ht_insert(&ht, keys[i], values[i]);
    }
    ht_print(&ht);

    // Busca
    int out;
    int q[] = {52, 999, 137};
    for (int i = 0; i < 3; ++i) {
        if (ht_search(&ht, q[i], &out))
            printf("Busca %d -> encontrado, valor=%d\n", q[i], out);
        else
            printf("Busca %d -> NÃO encontrado\n", q[i]);
    }

    // Remoção e reinserção
    printf("\nRemovendo 69 e 86...\n");
    ht_remove(&ht, 69);
    ht_remove(&ht, 86);
    ht_print(&ht);

    printf("Reinserindo 86 com novo valor 9999...\n");
    ht_insert(&ht, 86, 9999);
    ht_print(&ht);
}

int main(void) {
    // Demonstra os 3 métodos: 1) Linear, 2) Quadrática, 3) Duplo Hash
    demo(LINEAR);
    demo(QUADRATIC);
    demo(DOUBLE_HASH);
    return 0;
}