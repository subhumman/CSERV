/*
 * BORROWED CODE - Hash Table Implementation
 * This file contains a hash table implementation using binary search trees
 * for collision resolution, borrowed from external library code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "hash.h"
#include "tree.h"
#include "type.h"

// Hash table structure containing type information and array of trees
typedef struct HashTab {
    struct {
        vtype_t key;    // Type of key values
        vtype_t value;  // Type of value data
    } type;
    size_t size;        // Size of the hash table (number of buckets)
    Tree **table;       // Array of binary search trees for collision resolution
} HashTab;

// Function prototypes for internal hash operations
static uint32_t _get_hash(HashTab *hashtab, void *key);
static uint32_t _strhash(char *s, size_t size);

// Create a new hash table with specified size and key/value types
extern HashTab *new_hashtab(size_t size, vtype_t key, vtype_t value) {
    // Validate key type - only decimal and string types are supported
    switch(key){
        case DECIMAL_TYPE:
        case STRING_TYPE:
            break;
        default:
            fprintf(stderr, "%s\n", "key type not supported");
            return NULL;
    }
    // Validate value type - decimal, real, and string types are supported
    switch(value) {
        case DECIMAL_TYPE: 
        case REAL_TYPE: 
        case STRING_TYPE: 
            break;
        default:
            fprintf(stderr, "%s\n", "value type not supported");
            return NULL;
    }
    // Allocate and initialize hash table structure
    HashTab *hashtab = (HashTab*)malloc(sizeof(HashTab));
    hashtab->table = (Tree**)malloc(size * sizeof(Tree*));
    // Initialize each bucket with an empty binary search tree
    for (size_t i = 0; i < size; ++i) {
        hashtab->table[i] = new_tree(key, value);
    }
    hashtab->size = size;
    hashtab->type.key = key;
    hashtab->type.value = value;
    return hashtab;
}

// Delete a key-value pair from the hash table
extern void del_hashtab(HashTab *hashtab, void *key) {
    uint32_t hash = _get_hash(hashtab, key);
    del_tree(hashtab->table[hash], key);
}

// Check if a key exists in the hash table
extern _Bool in_hashtab(HashTab *hashtab, void *key) {
    uint32_t hash = _get_hash(hashtab, key);
    return in_tree(hashtab->table[hash], key);
}

// Get value associated with a key from the hash table
extern value_t get_hashtab(HashTab *hashtab, void *key) {
    uint32_t hash = _get_hash(hashtab, key);
    return get_tree(hashtab->table[hash], key);
}

// Set or update a key-value pair in the hash table
extern int8_t set_hashtab(HashTab *hashtab, void *key, void *value) {
    uint32_t hash = _get_hash(hashtab, key);
    set_tree(hashtab->table[hash], key, value);
    return 0;
}

// Compare two hash tables for equality
extern _Bool eq_hashtab(HashTab *x, HashTab *y) {
    if (x->type.key != y->type.key) {
        return 0;
    }
    if (x->type.value != y->type.value) {
        return 0;
    }
    if (x->size != y->size) {
        return 0;
    }
    // Compare each bucket (tree) for equality
    for (size_t i = 0; i < x->size; ++i) {
        if (size_tree(x->table[i]) != size_tree(y->table[i])) {
            return 0;
        }
        if (eq_tree(x->table[i], y->table[i]) != 0) {
            return 0;
        }
    }
    return 1;
}

// Get the size of the hash table (number of buckets)
extern size_t size_hashtab(HashTab *hashtab) {
    return hashtab->size;
}

// Get the size of the HashTab structure
extern size_t sizeof_hashtab(void) {
    return sizeof(HashTab);
}

// Free all memory allocated for the hash table
extern void free_hashtab(HashTab *hashtab) {
    // Free each binary search tree in the table
    for (size_t i = 0; i < hashtab->size; ++i) {
        free_tree(hashtab->table[i]);
    }
    free(hashtab->table);
    free(hashtab);
}

// Print hash table contents in compact format
extern void print_hashtab(HashTab *hashtab) {
    printf("#H[ ");
    for (size_t i = 0; i < hashtab->size; ++i) {
        if (size_tree(hashtab->table[i]) == 0) {
            continue;
        }
        printf("(%u :: ", (uint32_t)i);
        print_tree(hashtab->table[i]);
        printf(") ");
    }
    putchar(']');
}

// Print hash table contents with newline
extern void println_hashtab(HashTab *hashtab) {
    print_hashtab(hashtab);
    putchar('\n');
}

// Print hash table contents in formatted layout
extern void print_hashtab_format(HashTab *hashtab) {
    printf("#H[\n");
    for (size_t i = 0; i < hashtab->size; ++i) {
        if (size_tree(hashtab->table[i]) == 0) {
            continue;
        }
        printf("\t(%u :: ", (uint32_t)i);
        print_tree(hashtab->table[i]);
        printf(")\n");
    }
    putchar(']');
}

// Print hash table contents in formatted layout with newline
extern void println_hashtab_format(HashTab *hashtab) {
    print_hashtab_format(hashtab);
    putchar('\n');
}

// Calculate hash value for a key based on its type
static uint32_t _get_hash(HashTab *hashtab, void *key) {
    uint32_t hash = 0;
    switch(hashtab->type.key) {
        case DECIMAL_TYPE:
            // Use modulo for integer keys
            hash = (uint32_t)(intptr_t)key % hashtab->size;
        break;
        case STRING_TYPE:
            // Use string hash function for string keys
            hash = _strhash((char*)key, hashtab->size);
        break;
        default: ;
    }
    return hash;
}

// Simple string hash function using polynomial rolling hash
static uint32_t _strhash(char *s, size_t size) {
    uint32_t hashval;
    for (hashval = 0; *s != '\0'; ++s) {
        hashval = *s + 31 * hashval;
    }
    return hashval % size;
}
