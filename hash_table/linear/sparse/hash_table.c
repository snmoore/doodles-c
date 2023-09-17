// Hash table using a linear, sparse approach.
//
// This is implemented as fixed size array where each key indexes directly into the array without collision resolution.
//
// Hence:
//  Capacity        : 2^k where k is the number of bits in the key.
//  Time complexity : O(1)
//  Memory usage    : O(n) where n is the capacity.

#include <errno.h>          // For strerror
#include <stdio.h>          // For printf
#include <stdlib.h>         // For malloc
#include <string.h>         // For strerror
#include "hash_table.h"     // This module


// Create a hash table.
//
// This allocates and initialises all memory for the hash table.
//
// Parameters:
//  key_bits  : number of bits in each key.
//  item_size : maximum size of an item that will be inserted into the hash table, in bytes.
//  returns   : pointer to the hash table.
hash_table_t * hash_table_create(hash_key_bits_t key_bits, size_t item_size) {
    // Allocate the table.
    hash_table_t * table = malloc(sizeof(hash_table_t));
    if(table == NULL) {
        printf("Failed to allocate table: %s", strerror(errno));
        return NULL;
    }

    // Set the metadata.
    table->key_bits  = key_bits;
    table->capacity  = 1 << key_bits;
    table->item_size = item_size;

    // Allocate space for the array of flags that tracks which keys are present in the table.
    table->present = calloc(table->capacity, table->item_size);
    if(table->present == NULL) {
        printf("Failed to allocate table flags: %s", strerror(errno));
        free(table);
        return NULL;
    }

    // Allocate space for the array of items.
    table->items = calloc(table->capacity, table->item_size);
    if(table->present == NULL) {
        printf("Failed to allocate table items: %s", strerror(errno));
        free(table->present);
        free(table);
        return NULL;
    }

    return table;
}


// Destroy a hash table.
//
// This frees all allocated memory for the hash table.
//
// Parameters:
//  table : pointer to the hash table.
void hash_table_destroy(hash_table_t ** table) {
    free((*table)->items);
    free((*table)->present);
    free(*table);
    *table = NULL;
}


// Insert an item into a hash table.
//
// Parameters:
//  table     : pointer to the hash table.
//  key       : key for the item to be inserted.
//  item_size : size of the item to be inserted, in bytes.
//  data      : pointer to the item to be inserted.
void hash_table_insert(hash_table_t * table, uint16_t key, size_t item_size, const void * const item) {
    // Mark the key as being present.
    table->present[key] = true;

    // Copy the item into the table.
    memcpy(&table->items[key], item, item_size);
}


// Get an item from a hash table.
//
// Parameters:
//  table     : pointer to the hash table.
//  key       : key for the item to be retrieved.
//  item_size : size of the item to be retrieved, in bytes.
//  data      : pointer into which the item will be retrieved inserted.
//  returns   : true if the key was present and the value was retrieved, else false.
bool hash_table_get(hash_table_t * table, uint16_t key, size_t item_size, void * const item) {
    if(table->present[key]) {
        memcpy(&table->items[key], item, item_size);
        return true;
    }
    return false;
}
