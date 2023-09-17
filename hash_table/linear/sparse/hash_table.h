// Hash table using a linear, sparse approach.
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

// Valid numbers of bits in a key.
typedef enum hash_key_bits_tag {
    HASH_KEY_BITS_16 = 16
} hash_key_bits_t;

// Hash table.
//
// Fields:
//  key_bits    : number of bits in each key.
//  capacity    : capacity of the hash table i.e. 2^key_bits.
//  bucket_size : size of each bucket in the hash table, in bytes.
//  buckets     : array of buckets, will be allocated when the hash table is created.
//  present     : array of flags that tracks which keys are present, will be allocated when the hash table is created.
typedef struct hash_table_tag {
    hash_key_bits_t key_bits;
    size_t          capacity;
    size_t          bucket_size;
    uint8_t *       buckets;
    bool *          present;
} hash_table_t;

// Create a hash table.
//
// This allocates and initialises all memory for the hash table.
//
// Parameters:
//  key_bits  : number of bits in each key.
//  item_size : maximum size of an item that will be inserted into the hash table, in bytes.
//  returns   : pointer to the hash table.
hash_table_t * hash_table_create(hash_key_bits_t key_bits, size_t item_size);

// Destroy a hash table.
//
// This frees all allocated memory for the hash table.
//
// Parameters:
//  table : pointer to the hash table.
void hash_table_destroy(hash_table_t ** table);

// Insert an item into a hash table.
//
// Parameters:
//  table     : pointer to the hash table.
//  key       : key for the item to be inserted.
//  item_size : size of the item to be inserted, in bytes.
//  data      : pointer to the item to be inserted.
void hash_table_insert(hash_table_t * table, uint16_t key, size_t item_size, const void * const item);

// Delete an item from a hash table.
//
// Parameters:
//  table     : pointer to the hash table.
//  key       : key for the item to be deleted.
//  returns   : true if the key was present and the item was deleted.
bool hash_table_delete(hash_table_t * table, uint16_t key);

// Retrieve an item from a hash table.
//
// Parameters:
//  table     : pointer to the hash table.
//  key       : key for the item to be retrieved.
//  item_size : size of the item to be retrieved, in bytes.
//  item      : pointer into which the item will be retrieved.
//  returns   : true if the key was present and the item was retrieved.
//              false if the key was not present, item remains unchanged.
bool hash_table_retrieve(hash_table_t * table, uint16_t key, size_t item_size, void * const item);

#endif
