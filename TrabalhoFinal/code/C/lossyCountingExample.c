#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

// Define item‐type (you may use e.g., int or long or string pointers)
typedef int ItemType;

typedef struct {
    ItemType item;
    long count;
    long delta;
} LCEntry;

typedef struct {
    LCEntry *entries;
    size_t capacity;
    size_t size;
} LCTable;

LCTable *lc_create(size_t capacity) {
    LCTable *T = malloc(sizeof(LCTable));
    T->entries = malloc(capacity * sizeof(LCEntry));
    T->capacity = capacity;
    T->size = 0;
    return T;
}

void lc_free(LCTable *T) {
    free(T->entries);
    free(T);
}

// You need a lookup (hash table) of item → index in entries
// Simplest: if item domain small, use array; else implement hash map.

// Insert or increment
void lc_process_item(LCTable *T, ItemType x, long current_bucket) {
    // 1) try to find x in T->entries
    size_t i;
    for (i = 0; i < T->size; i++) {
        if (T->entries[i].item == x) {
            T->entries[i].count++;
            return;
        }
    }
    // 2) if not found, insert new entry
    if (T->size < T->capacity) {
        T->entries[T->size].item = x;
        T->entries[T->size].count = 1;
        T->entries[T->size].delta = current_bucket - 1;
        T->size++;
    } else {
        // capacity exceeded — you may need to prune now or before
    }
}

// Prune phase: after processing each bucket width number of items
void lc_prune(LCTable *T, long current_bucket) {
    size_t j = 0;
    for (size_t i = 0; i < T->size; i++) {
        if (T->entries[i].count + T->entries[i].delta > current_bucket) {
            // keep it
            T->entries[j++] = T->entries[i];
        }
        // else drop
    }
    T->size = j;
}

// At end: output items with count ≥ support_threshold * N
void lc_output(LCTable *T, long N, double support_threshold, double epsilon) {
    long threshold = (long)ceil(support_threshold * N);
    for (size_t i = 0; i < T->size; i++) {
        if (T->entries[i].count >= threshold) {
            printf("Item %d: estimated count %ld (true >= %ld-epsilonN)\n",
                   T->entries[i].item,
                   T->entries[i].count,
                   threshold);
        }
    }
}

int main(void) {
    double epsilon = 0.01;
    double support = 0.02; // e.g., want items that exceed 2% of stream
    long bucket_width = (long)ceil(1.0 / epsilon);
    LCTable *T = lc_create(10000); // capacity choose large enough
    long N = 0;
    long current_bucket = 1;

    // Example: stream of items from stdin or file
    ItemType x;
    while (scanf("%d", &x) == 1) {
        N++;
        lc_process_item(T, x, current_bucket);
        if (N % bucket_width == 0) {
            lc_prune(T, current_bucket);
            current_bucket++;
        }
    }

    // Final prune optionally
    lc_prune(T, current_bucket);

    lc_output(T, N, support, epsilon);

    lc_free(T);
    return 0;
}
