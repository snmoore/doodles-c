// Ceedling tests for hash table using a linear, sparse approach.
//
// This is implemented as fixed size array where each key indexes directly into the array without collision resolution.
//
// Tests:
//  1.  Create a hash table.
//  2.  Destroy a hash table.
//  3.  Insert an item into a hash table.
//  4a. Delete an item from a hash table -- key not present.
//  4b. Delete an item from a hash table -- key present.
//  5a. Retrieve an item from a hash table -- key not present.
//  5b. Retrieve an item from a hash table -- key present.
//  6a. Insert and retrieve multiple items from a hash table -- two items.
//  6b. Insert and retrieve multiple items from a hash table -- exhaustive.

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
    TEST_ASSERT_EQUAL_size_t(2,     table->bucket_size);

    // Verify the array of flags that tracks which keys are present in the table.
    TEST_ASSERT_NOT_NULL(table->present);
    const bool present = false;
    TEST_ASSERT_EACH_EQUAL_MEMORY(&present, table->present, sizeof(present), table->capacity);

    // Verify the array of items.
    TEST_ASSERT_NOT_NULL(table->buckets);
    const item_t item = 0;
    TEST_ASSERT_EACH_EQUAL_MEMORY(&item, table->buckets, sizeof(item), table->capacity);

    // Cleanup: No destroy because we haven't tested that functionality yet.
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
    const item_t   item = 3;
    hash_table_insert(table, key, sizeof(item), &item);

    // Verify the hash table contents.
    TEST_ASSERT_TRUE(table->present[key]);
    TEST_ASSERT_EQUAL_UINT16(item, *(uint16_t*)(table->buckets + (key * sizeof(uint16_t))));

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 4a. Delete an item from a hash table -- key not present.
void test_4a_hash_table_delete_not_present(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Delete an item from a hash table -- key not present.
    const uint16_t key = 3;
    const bool deleted = hash_table_delete(table, key);
    TEST_ASSERT_FALSE(deleted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 4b. Delete an item from a hash table -- key present.
void test_4b_hash_table_delete_present(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert an item into a hash table.
    const uint16_t key  = 3;
    const item_t   item = 3;
    hash_table_insert(table, key, sizeof(item), &item);

    // Test: Delete an item from a hash table -- key present.
    const bool deleted = hash_table_delete(table, key);
    TEST_ASSERT_TRUE(deleted);

    // Verify the hash table contents.
    TEST_ASSERT_FALSE(table->present[key]);
    TEST_ASSERT_EQUAL_UINT16(0, *(uint16_t*)(table->buckets + (key * sizeof(uint16_t))));

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 5a. Retrieve an item from a hash table -- key not present.
void test_5a_hash_table_retrieve_not_present(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Retrieve an item from a hash table -- key not present.
    const uint16_t key = 3;
    item_t item = 0;
    const bool present = hash_table_retrieve(table, key, sizeof(item), &item);
    TEST_ASSERT_FALSE(present);
    TEST_ASSERT_EQUAL_UINT16(0, item);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 5b. Retrieve an item from a hash table -- key present.
void test_5b_hash_table_retrieve_present(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert an item into a hash table.
    const uint16_t key      = 3;
    const item_t   inserted = 3;
    hash_table_insert(table, key, sizeof(inserted), &inserted);

    // Test: Retrieve an item from a hash table -- key present.
    item_t retrieved;
    const bool present = hash_table_retrieve(table, key, sizeof(retrieved), &retrieved);
    TEST_ASSERT_TRUE(present);
    TEST_ASSERT_EQUAL_UINT16(inserted, retrieved);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 6a. Insert and retrieve multiple items from a hash table -- two items.
void test_6a_hash_table_insert_retrieve_multiple_two(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: 1st item.
    {
        // Insert an item into a hash table.
        const uint16_t key      = 3;
        const item_t   inserted = 3;
        hash_table_insert(table, key, sizeof(inserted), &inserted);

        // Retrieve an item from a hash table.
        item_t retrieved;
        const bool present = hash_table_retrieve(table, key, sizeof(retrieved), &retrieved);
        TEST_ASSERT_TRUE(present);
        TEST_ASSERT_EQUAL_UINT16(inserted, retrieved);

        // Verify the hash table contents.
        TEST_ASSERT_TRUE(table->present[key]);
        TEST_ASSERT_EQUAL_UINT16(inserted, *(uint16_t*)(table->buckets + (key * sizeof(uint16_t))));
    }

    // Test: 2nd item.
    {
        // Insert an item into a hash table.
        const uint16_t key      = 7;
        const item_t   inserted = 7;
        hash_table_insert(table, key, sizeof(inserted), &inserted);

        // Retrieve an item from a hash table.
        item_t retrieved;
        const bool present = hash_table_retrieve(table, key, sizeof(retrieved), &retrieved);
        TEST_ASSERT_TRUE(present);
        TEST_ASSERT_EQUAL_UINT16(inserted, retrieved);

        // Verify the hash table contents.
        TEST_ASSERT_TRUE(table->present[key]);
        TEST_ASSERT_EQUAL_UINT16(inserted, *(uint16_t*)(table->buckets + (key * sizeof(uint16_t))));
    }

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 6b. Insert and retrieve multiple items from a hash table -- exhaustive.
void test_6b_hash_table_insert_retrieve_multiple_exhaustive(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(item_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Insert multiple items into a hash table -- exhaustive.
    for(uint16_t key = 0; key < 65535; key++) {
        const item_t item = key;
        hash_table_insert(table, key, sizeof(item), &item);
    }

    // Test: Retrieve multiple items from a hash table -- exhaustive.
    for(uint16_t key = 0; key < 65535; key++) {
        item_t retrieved;
        const bool present = hash_table_retrieve(table, key, sizeof(retrieved), &retrieved);
        TEST_ASSERT_TRUE(present);
        TEST_ASSERT_EQUAL_UINT16(key, retrieved);
    }

    // Verify the hash table contents.
    for(uint16_t key = 0; key < 65535; key++) {
        TEST_ASSERT_TRUE(table->present[key]);
        TEST_ASSERT_EQUAL_UINT16(key, *(uint16_t*)(table->buckets + (key * sizeof(uint16_t))));
    }

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}
