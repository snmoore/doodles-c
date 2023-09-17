// Ceedling tests for hash table using a linear, sparse approach.
//
// This is implemented as fixed size array where each key indexes directly into the array without collision resolution.
//
// Tests:
//  1.  Create a hash table.
//  2.  Destroy a hash table.
//  3.  Insert an item into a hash table.
//  4a. Get an item from a hash table -- key not present.
//  4b. Get an item from a hash table -- key present.

#include "unity.h"      // Unity test framework
#include "hash_table.h" // Unit under test

// Type for an item to be stored in the table.
typedef uint16_t item_t;

// Test 1. Create a hash table.
void test_1_hash_table_create(void) {
    // Test: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));

    // Verify the hash table and its metadata.
    TEST_ASSERT_NOT_NULL(table);
    TEST_ASSERT_EQUAL_INT(   16,    table->key_bits);
    TEST_ASSERT_EQUAL_size_t(65536, table->capacity);
    TEST_ASSERT_EQUAL_size_t(2,     table->item_size);

    // Verify the array of flags that tracks which keys are present in the table.
    TEST_ASSERT_NOT_NULL(table->present);
    const bool present = false;
    TEST_ASSERT_EACH_EQUAL_MEMORY(&present, table->present, sizeof(present), table->capacity);

    // Verify the array of items.
    TEST_ASSERT_NOT_NULL(table->items);
    const item_t item = 0;
    TEST_ASSERT_EACH_EQUAL_MEMORY(&item, table->items, sizeof(item), table->capacity);
}

// Test 2. Destroy a hash table.
void test_2_hash_table_destroy(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Destroy a hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 3. Insert an item into a hash table.
void test_3_hash_table_insert(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Insert an item into a hash table.
    const uint16_t key  = 3;
    const item_t   item = 23497;
    hash_table_insert(table, key, sizeof(item), &item);
    TEST_ASSERT_TRUE(table->present[key]);
//    TEST_ASSERT_EQUAL_UINT16(23497, ((item_t*)table->items)[key]);
//    TEST_ASSERT_EQUAL_UINT16(23497, *((item_t*)table->items + key));

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 4a. Get an item from a hash table -- key not present.
void test_4a_hash_table_get_not_present(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Get an item from a hash table -- key not present.
    const uint16_t key = 3;
    item_t item = 0;
    const bool present = hash_table_get(table, key, sizeof(item), &item);
    TEST_ASSERT_FALSE(present);
    TEST_ASSERT_EQUAL_UINT16(0, item);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 4b. Get an item from a hash table -- key present.
void test_4b_hash_table_get_present(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert an item into a hash table.
    const uint16_t key      = 3;
    const item_t   inserted = 23497;
    hash_table_insert(table, key, sizeof(inserted), &inserted);

    // Test: Get the item from a hash table -- key present.
    item_t retrieved;
    const bool present = hash_table_get(table, key, sizeof(retrieved), &retrieved);
    TEST_ASSERT_TRUE(present);
    TEST_ASSERT_EQUAL_UINT16(inserted, retrieved);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}
