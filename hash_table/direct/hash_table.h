// Hash table using direct addressing.
//
// This is implemented as fixed size array where each key indexes directly into the array without collision resolution.
//
// Hence:
//  Capacity        : 2^k where k is the number of bits in the key.
//  Time complexity : O(1)
//  Memory usage    : O(n) where n is the capacity.

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>    // For bool
#include <stddef.h>     // For size_t
#include <stdint.h>     // For uint16_t

// Opaque type for a hash table.
typedef struct hash_table_tag hash_table_t;

// Valid numbers of bits in a key.
typedef enum hash_key_bits_tag {
    HASH_KEY_BITS_16 = 16
} hash_key_bits_t;

// Create a hash table i.e. allocate and initialise all memory.
//
// Parameters:
//  key_bits   : number of bits in each key.
//  value_size : maximum size of a value that will be inserted into the hash table, in bytes.
//
// Returns:
//  pointer to the hash table or NULL if memory could not be allocated.
hash_table_t * hash_table_create(hash_key_bits_t key_bits, size_t value_size);

// Destroy a hash table i.e. free all allocated memory.
//
// Parameters:
//  table : pointer to pointer to the hash table.
void hash_table_destroy(hash_table_t ** table);

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
                       bool overwrite);

// Delete a value from a hash table.
//
// Parameters:
//  table : pointer to the hash table.
//  key   : key for the value to be deleted.
//
// Returns:
//  true  : the key was present, the value was deleted.
//  false : the key was not present.
bool hash_table_delete(hash_table_t * const table, uint16_t key);

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
bool hash_table_retrieve(const hash_table_t * const table, uint16_t key, size_t value_size, void * const value);

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
typedef void (*hash_table_iterate_callback_t)(uint16_t key, void * const value);
void hash_table_iterate(const hash_table_t * const table, size_t value_size, void * const value,
                        hash_table_iterate_callback_t callback);

#endif // HASH_TABLE_H
