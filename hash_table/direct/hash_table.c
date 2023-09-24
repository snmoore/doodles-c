// Hash table using direct addressing.
//
// This is implemented as fixed size array where each key indexes directly into the array without collision resolution.
//
// Hence:
//  Capacity        : 2^k where k is the number of bits in the key.
//  Time complexity : O(1)
//  Memory usage    : O(n) where n is the capacity.

#include <assert.h>         // For assert
#include <errno.h>          // For errno
#include <stdio.h>          // For printf
#include <stdlib.h>         // For malloc
#include <string.h>         // For strerror
#include "hash_table.h"     // This module

// Concrete type for a hash table, corresponding to typedef hash_table_t.
//
// Fields:
//  key_bits    : number of bits in each key.
//  capacity    : capacity of the hash table i.e. 2^key_bits.
//  bucket_size : size of each bucket in the hash table, in bytes.
//  buckets     : array of buckets, will be allocated when the hash table is created.
//  present     : array of flags that tracks which keys are present, will be allocated when the hash table is created.
struct hash_table_tag {
    hash_key_bits_t key_bits;
    size_t          capacity;
    size_t          bucket_size;
    uint8_t *       buckets;
    bool *          present;
};

// Create a hash table i.e. allocate and initialise all memory.
//
// Parameters:
//  key_bits   : number of bits in each key.
//  value_size : maximum size of a value that will be inserted into the hash table, in bytes.
//
// Returns:
//  pointer to the hash table or NULL if memory could not be allocated.
hash_table_t * hash_table_create(hash_key_bits_t key_bits, size_t value_size) {
    // Allocate the table.
    hash_table_t * table = malloc(sizeof(hash_table_t));
    if(table == NULL) {
        printf("Failed to allocate table: %s", strerror(errno));
        return NULL;
    }

    // Set the metadata.
    table->key_bits    = key_bits;
    table->capacity    = 1 << key_bits;
    table->bucket_size = value_size;

    // Allocate space for the array of buckets.
    table->buckets = calloc(table->capacity, table->bucket_size);
    if(table->buckets == NULL) {
        printf("Failed to allocate buckets: %s", strerror(errno));
        free(table);
        return NULL;
    }

    // Allocate space for the array of flags that tracks which keys are present in the table.
    table->present = calloc(table->capacity, table->bucket_size);
    if(table->present == NULL) {
        printf("Failed to allocate flags: %s", strerror(errno));
        free(table->buckets);
        free(table);
        return NULL;
    }

    return table;
}

// Destroy a hash table i.e. free all allocated memory.
//
// Parameters:
//  table : pointer to pointer to the hash table.
void hash_table_destroy(hash_table_t ** table) {
    assert(table != NULL);

    free((*table)->buckets);
    free((*table)->present);
    free(*table);
    *table = NULL;
}

// Insert a value into a hash table.
//
// Parameters:
//  table      : pointer to the hash table.
//  key        : key for the value to be inserted.
//  value_size : size of the value to be inserted, in bytes.
//  value      : pointer to the value to be inserted.
//  overwrite  : true if the value should be overwritten if the key is already present.
//
// Returns:
//  true       : the value was inserted.
//  false      : the value was not inserted i.e. the key is already present and overwrite is disallowed.
bool hash_table_insert(hash_table_t * const table, uint16_t key, size_t value_size, const void * const value,
                       bool overwrite) {
    assert(table      != NULL);
    assert(value_size != 0);
    assert(value_size <= table->bucket_size);
    assert(value      != NULL);

    // Only overwrite if allowed.
    if(overwrite || !table->present[key]) {
        // Copy the value into the bucket.
        const size_t offset = key * table->bucket_size;
        memcpy(table->buckets + offset, value, value_size);

        // Mark the key as being present.
        table->present[key] = true;
        return true;
    }

    // Key is already present and overwrite is disallowed.
    return false;
}

// Delete a value from a hash table.
//
// Parameters:
//  table : pointer to the hash table.
//  key   : key for the value to be deleted.
//
// Returns:
//  true  : the key was present, the value was deleted.
//  false : the key was not present.
bool hash_table_delete(hash_table_t * const table, uint16_t key) {
    assert(table != NULL);

    // Delete the value.
    if(table->present[key]) {
        // Clear the bucket.
        const size_t offset = key * table->bucket_size;
        memset(table->buckets + offset, 0, table->bucket_size);

        // Mark the key as not present.
        table->present[key] = false;
        return true;
    }
    return false;
}

// Retrieve a value from a hash table.
//
// Parameters:
//  table      : pointer to the hash table.
//  key        : key for the value to be retrieved.
//  value_size : size of the value to be retrieved, in bytes.
//  value      : pointer into which the value will be retrieved.
//
// Returns:
//  true       : the key was present, the value was retrieved.
//  false      : the key was not present.
bool hash_table_retrieve(const hash_table_t * const table, uint16_t key, size_t value_size, void * const value) {
    assert(table      != NULL);
    assert(value_size != 0);
    assert(value_size <= table->bucket_size);
    assert(value      != NULL);

    // Retrieve the value.
    if(table->present[key]) {
        // Copy the value from the bucket.
        const size_t offset = key * table->bucket_size;
        memcpy(value, table->buckets + offset, value_size);
        return true;
    }
    return false;
}

// Iterate over all keys that are present in a hash table.
//
// For each key that is present in the hash table:
//  1. Retrieve the value into the provided value argument.
//  2. Call the callback function with the key and the provided value argument.
//
// Parameters:
//  table      : pointer to the hash table.
//  value_size : size of the value to be retrieved, in bytes.
//  value      : pointer into which each value will be retrieved.
//  callback   : function to be called for each value that is retrieved.
void hash_table_iterate(const hash_table_t * const table, size_t value_size, void * const value,
                        hash_table_iterate_callback_t callback) {
    assert(table      != NULL);
    assert(value_size != 0);
    assert(value_size <= table->bucket_size);
    assert(value      != NULL);
    assert(callback   != NULL);

    // Iterate over the hash table.
    for(size_t key = 0; key < table->capacity; key++) {
        if(table->present[key]) {
            // Copy the value from the bucket.
            const size_t offset = key * table->bucket_size;
            memcpy(value, table->buckets + offset, value_size);

            // Call the callback function.
            callback((uint16_t)key, value);
        }
    }
}
