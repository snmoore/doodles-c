// Ceedling tests for hash table.
//
// Tests:
//  1.  Create a hash table.
//
//  2a. Destroy a hash table -- fail, null table.
//  2b. Destroy a hash table -- success.
//
//  3a. Insert a value into a hash table -- fail, null table.
//  3b. Insert a value into a hash table -- fail, zero size value.
//  3c. Insert a value into a hash table -- fail, value size > bucket size.
//  3d. Insert a value into a hash table -- fail, null value.
//  3e. Insert a value into a hash table -- success.
//  3f. Insert a value into a hash table -- success, value size < bucket size.
//
//  4a. Insert a value into a hash table when the key is already present -- fail, overwrite disallowed.
//  4b. Insert a value into a hash table when the key is already present -- success, overwrite allowed.
//
//  5a. Delete a value from a hash table -- fail, null table.
//  5b. Delete a value from a hash table -- fail, key not present.
//  5c. Delete a value from a hash table -- success.
//
//  6a. Retrieve a value from a hash table -- fail, null table.
//  6b. Retrieve a value from a hash table -- fail, zero size value.
//  6c. Retrieve a value from a hash table -- fail, value size > bucket size.
//  6d. Retrieve a value from a hash table -- fail, null value.
//  6e. Retrieve a value from a hash table -- fail, key not present.
//  6f. Retrieve a value from a hash table -- success.
//  6g. Retrieve a value from a hash table -- success, value size < bucket size.
//
//  7a. Insert and retrieve multiple values from a hash table -- two values.
//  7b. Insert and retrieve multiple values from a hash table -- exhaustive.
//
//  8a. Iterate over all keys that are present in a hash table -- fail, null table.
//  8b. Iterate over all keys that are present in a hash table -- fail, zero size value.
//  8c. Iterate over all keys that are present in a hash table -- fail, value size > bucket size.
//  8d. Iterate over all keys that are present in a hash table -- fail, null value.
//  8e. Iterate over all keys that are present in a hash table -- fail, null callback.
//  8f. Iterate over all keys that are present in a hash table -- success.

#include "unity.h"          // Unity test framework
#include "hash_table.h"     // Unit under test
#include "expect_assert.h"  // Support for expecting assert() failures.

// Type for a value to be stored in the table.
typedef uint16_t value_t;

// Setup that is run before every test.
void setUp(void) {
    // Do not expect an assert() failure.
    expect_assert_clear();
}

// Test 1. Create a hash table.
void test_1_hash_table_create(void) {
    // Test: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Cleanup: No destroy because we haven't tested that functionality yet.
}

// Test 2a. Destroy a hash table -- fail, null table.
void test_2a_hash_table_destroy_fail_null_table(void) {
    // Expect an assert() failure.
    expect_assert();

    // Test: Destroy a hash table -- fail, null table.
    hash_table_destroy(NULL);
}

// Test 2b. Destroy a hash table -- success.
void test_2b_hash_table_destroy_success(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Destroy a hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 3a. Insert a value into a hash table -- fail, null table.
void test_3a_hash_table_insert_fail_null_table(void) {
    // Expect an assert() failure.
    expect_assert();

    // Test: Insert a value into a hash table -- fail, null table.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(NULL, key, sizeof(value), &value, false);
    TEST_ASSERT_FALSE(inserted);
}

// Test 3b. Insert a value into a hash table -- fail, zero size value.
void test_3b_hash_table_insert_fail_zero_size_value(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Expect an assert() failure.
    expect_assert();

    // Test: Insert a value into a hash table -- fail, zero size value.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(table, key, 0, &value, false);
    TEST_ASSERT_FALSE(inserted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 3c. Insert a value into a hash table -- fail, value size > bucket size.
void test_3c_hash_table_insert_fail_value_size_gt_bucket_size(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Expect an assert() failure.
    expect_assert();

    // Test: Insert a value into a hash table -- fail, value size > bucket size.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(table, key, sizeof(value) + 1, &value, false);
    TEST_ASSERT_FALSE(inserted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 3d. Insert a value into a hash table -- fail, null value.
void test_3d_hash_table_insert_fail_null_value(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Expect an assert() failure.
    expect_assert();

    // Test: Insert a value into a hash table -- fail, null value.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(table, key, sizeof(value), NULL, false);
    TEST_ASSERT_FALSE(inserted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 3e. Insert a value into a hash table -- success.
void test_3e_hash_table_insert_success(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Insert a value into a hash table -- success.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 3f. Insert a value into a hash table -- success, value size < bucket size.
void test_3f_hash_table_insert_success_value_size_lt_bucket_size(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Insert a value into a hash table -- success, value size < bucket size.
    const uint16_t key        = 3;
    const value_t  value      = 0xaa55;
    const size_t   value_size = 1;
    const bool inserted = hash_table_insert(table, key, value_size, &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 4a. Insert a value into a hash table when the key is already present -- fail, overwrite disallowed.
void test_4a_hash_table_insert_key_already_present_fail_overwrite_disallowed(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert a value into a hash table.
    const uint16_t key   = 3;
    const value_t  value = 3;
    bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Test: Insert a value into a hash table when the key is already present -- fail, overwrite disallowed.
    const value_t new_value = 7;
    inserted = hash_table_insert(table, key, sizeof(new_value), &new_value, false);
    TEST_ASSERT_FALSE(inserted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 4b. Insert a value into a hash table when the key is already present -- success, overwrite allowed.
void test_4b_hash_table_insert_key_already_present_success_overwrite_allowed(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert a value into a hash table.
    const uint16_t key   = 3;
    const value_t  value = 3;
    bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Test: Insert a value into a hash table when the key is already present -- success, overwrite allowed.
    const value_t new_value = 7;
    inserted = hash_table_insert(table, key, sizeof(new_value), &new_value, true);
    TEST_ASSERT_TRUE(inserted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 5a. Delete a value from a hash table -- fail, null table.
void test_5a_hash_table_delete_fail_null_table(void) {
    // Expect an assert() failure.
    expect_assert();

    // Test: Delete a value from a hash table -- fail, null table.
    const uint16_t key = 3;
    const bool deleted = hash_table_delete(NULL, key);
    TEST_ASSERT_FALSE(deleted);
}

// Test 5b. Delete a value from a hash table -- fail, key not present.
void test_5b_hash_table_delete_fail_key_not_present(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Delete a value from a hash table -- key not present.
    const uint16_t key = 3;
    const bool deleted = hash_table_delete(table, key);
    TEST_ASSERT_FALSE(deleted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 5c. Delete a value from a hash table -- success.
void test_5c_hash_table_delete_success(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert a value into a hash table.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Test: Delete a value from a hash table -- key present.
    const bool deleted = hash_table_delete(table, key);
    TEST_ASSERT_TRUE(deleted);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 6a. Retrieve a value from a hash table -- fail, null table.
void test_6a_hash_table_retrieve_fail_null_table(void) {
    // Expect an assert() failure.
    expect_assert();

    // Test: Retrieve a value from a hash table -- fail, null table.
    const uint16_t key = 3;
    value_t value = 0;
    const bool retrieved = hash_table_retrieve(NULL, key, sizeof(value), &value);
    TEST_ASSERT_FALSE(retrieved);
    TEST_ASSERT_EQUAL_UINT16(0, value);
}

// Test 6b. Retrieve a value from a hash table -- fail, zero size value.
void test_6b_hash_table_retrieve_fail_zero_size_value(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert a value into a hash table.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Expect an assert() failure.
    expect_assert();

    // Test: Retrieve a value from a hash table -- fail, zero size value.
    value_t value_retrieved = 0;
    const bool retrieved = hash_table_retrieve(table, key, 0, &value_retrieved);
    TEST_ASSERT_FALSE(retrieved);
    TEST_ASSERT_EQUAL_UINT16(0, value_retrieved);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 6c. Retrieve a value from a hash table -- fail, value size > bucket size.
void test_6c_hash_table_retrieve_fail_value_size_gt_bucket_size(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert a value into a hash table.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Expect an assert() failure.
    expect_assert();

    // Test: Retrieve a value from a hash table -- fail, value size > bucket size.
    value_t value_retrieved = 0;
    const bool retrieved = hash_table_retrieve(table, key, sizeof(value_retrieved) + 1, &value_retrieved);
    TEST_ASSERT_FALSE(retrieved);
    TEST_ASSERT_EQUAL_UINT16(0, value_retrieved);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 6d. Retrieve a value from a hash table -- fail, null value.
void test_6d_hash_table_retrieve_fail_null_value(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert a value into a hash table.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Expect an assert() failure.
    expect_assert();

    // Test: Retrieve a value from a hash table -- fail, null value.
    value_t value_retrieved = 0;
    const bool retrieved = hash_table_retrieve(table, key, sizeof(value_retrieved), NULL);
    TEST_ASSERT_FALSE(retrieved);
    TEST_ASSERT_EQUAL_UINT16(0, value_retrieved);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 6e. Retrieve a value from a hash table -- fail, key not present.
void test_6e_hash_table_retrieve_fail_key_not_present(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Retrieve a value from a hash table -- fail, key not present.
    const uint16_t key = 3;
    value_t value = 0;
    const bool retrieved = hash_table_retrieve(table, key, sizeof(value), &value);
    TEST_ASSERT_FALSE(retrieved);
    TEST_ASSERT_EQUAL_UINT16(0, value);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 6f. Retrieve a value from a hash table -- success.
void test_6f_hash_table_retrieve_success(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert a value into a hash table.
    const uint16_t key   = 3;
    const value_t  value = 3;
    const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Test: Retrieve a value from a hash table -- success.
    value_t value_retrieved;
    const bool retrieved = hash_table_retrieve(table, key, sizeof(value_retrieved), &value_retrieved);
    TEST_ASSERT_TRUE(retrieved);
    TEST_ASSERT_EQUAL_UINT16(value, value_retrieved);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 6g. Retrieve a value from a hash table -- success, value size < bucket size.
void test_6g_hash_table_retrieve_success_value_size_lt_bucket_size(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert a value into a hash table.
    const uint16_t key   = 3;
    const value_t  value = 0xaa55;
    const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
    TEST_ASSERT_TRUE(inserted);

    // Test: Retrieve a value from a hash table -- success, value size < bucket size.
    uint8_t value_retrieved = 0;
    const size_t retrieved_size = 1;
    const bool retrieved = hash_table_retrieve(table, key, retrieved_size, &value_retrieved);
    TEST_ASSERT_TRUE(retrieved);
    TEST_ASSERT_EQUAL_UINT16(value & 0xff, value_retrieved);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 7a. Insert and retrieve multiple values from a hash table -- two values.
void test_7a_hash_table_insert_retrieve_multiple_two(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: 1st value.
    {
        // Insert a value into a hash table.
        const uint16_t key   = 3;
        const value_t  value = 3;
        const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
        TEST_ASSERT_TRUE(inserted);

        // Retrieve a value from a hash table.
        value_t value_retrieved;
        const bool retrieved = hash_table_retrieve(table, key, sizeof(value_retrieved), &value_retrieved);
        TEST_ASSERT_TRUE(retrieved);
        TEST_ASSERT_EQUAL_UINT16(value, value_retrieved);
    }

    // Test: 2nd value.
    {
        // Insert a value into a hash table.
        const uint16_t key   = 7;
        const value_t  value = 7;
        const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
        TEST_ASSERT_TRUE(inserted);

        // Retrieve a value from a hash table.
        value_t value_retrieved;
        const bool retrieved = hash_table_retrieve(table, key, sizeof(value_retrieved), &value_retrieved);
        TEST_ASSERT_TRUE(retrieved);
        TEST_ASSERT_EQUAL_UINT16(value, value_retrieved);
    }

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 7b. Insert and retrieve multiple values from a hash table -- exhaustive.
void test_7b_hash_table_insert_retrieve_multiple_exhaustive(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Test: Insert multiple values into a hash table -- exhaustive.
    for(uint16_t key = 0; key < 65535; key++) {
        const value_t value = key;
        const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
        TEST_ASSERT_TRUE(inserted);
    }

    // Test: Retrieve multiple values from a hash table -- exhaustive.
    for(uint16_t key = 0; key < 65535; key++) {
        value_t value;
        const bool retrieved = hash_table_retrieve(table, key, sizeof(value), &value);
        TEST_ASSERT_TRUE(retrieved);
        TEST_ASSERT_EQUAL_UINT16(key, value);
    }

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 8 stub for the iteration callback function.
static void stub_8_callback_unexpected(uint16_t key, void * const value) {
    TEST_FAIL_MESSAGE("Did not expect the callback to be called.");
    (void)key;
    (void)value;
}

// Test 8a. Iterate over all keys that are present in a hash table -- fail, null table.
void test_8a_hash_table_iterate_fail_null_table(void) {
    // Expect an assert() failure.
    expect_assert();

    // Test: Iterate over all keys that are present in a hash table -- fail, null table.
    value_t value = 0;
    hash_table_iterate(NULL, sizeof(value), &value, stub_8_callback_unexpected);
    TEST_ASSERT_EQUAL_UINT16(0, value);
}

// Test 8b. Iterate over all keys that are present in a hash table -- fail, zero size value.
void test_8b_hash_table_iterate_fail_zero_size_value(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert multiple values into the hash table -- setting the value equal to the key.
    const uint16_t num_keys = 10;
    for(uint16_t key = 0; key < num_keys; key++) {
        const value_t value = key;
        const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
        TEST_ASSERT_TRUE(inserted);
    }

    // Expect an assert() failure.
    expect_assert();

    // Test: Iterate over all keys that are present in a hash table -- fail, zero size value.
    value_t value = 0;
    hash_table_iterate(table, 0, &value, stub_8_callback_unexpected);
    TEST_ASSERT_EQUAL_UINT16(0, value);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 8c. Iterate over all keys that are present in a hash table -- fail, value size > bucket size.
void test_8c_hash_table_iterate_fail_value_size_gt_bucket_size(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert multiple values into the hash table -- setting the value equal to the key.
    const uint16_t num_keys = 10;
    for(uint16_t key = 0; key < num_keys; key++) {
        const value_t value = key;
        const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
        TEST_ASSERT_TRUE(inserted);
    }

    // Expect an assert() failure.
    expect_assert();

    // Test: Iterate over all keys that are present in a hash table -- fail, value size > bucket size.
    value_t value = 0;
    hash_table_iterate(table, sizeof(value) + 1, &value, stub_8_callback_unexpected);
    TEST_ASSERT_EQUAL_UINT16(0, value);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 8d. Iterate over all keys that are present in a hash table -- fail, null value.
void test_8d_hash_table_iterate_fail_null_value(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert multiple values into the hash table -- setting the value equal to the key.
    const uint16_t num_keys = 10;
    for(uint16_t key = 0; key < num_keys; key++) {
        const value_t value = key;
        const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
        TEST_ASSERT_TRUE(inserted);
    }

    // Expect an assert() failure.
    expect_assert();

    // Test: Iterate over all keys that are present in a hash table --- fail, null value.
    value_t value = 0;
    hash_table_iterate(table, sizeof(value), NULL, stub_8_callback_unexpected);
    TEST_ASSERT_EQUAL_UINT16(0, value);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 8e. Iterate over all keys that are present in a hash table -- fail, null callback.
void test_8e_hash_table_iterate_fail_null_callback(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert multiple values into the hash table -- setting the value equal to the key.
    const uint16_t num_keys = 10;
    for(uint16_t key = 0; key < num_keys; key++) {
        const value_t value = key;
        const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
        TEST_ASSERT_TRUE(inserted);
    }

    // Expect an assert() failure.
    expect_assert();

    // Test: Iterate over all keys that are present in a hash table -- fail, null callback.
    value_t value = 0;
    hash_table_iterate(table, sizeof(value), &value, NULL);
    TEST_ASSERT_EQUAL_UINT16(0, value);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}

// Test 8f. Iterate over all keys that are present in a hash table -- success.
static uint16_t callback_8f_num_calls = 0;
static void callback_8f(uint16_t key, void * const value) {
    // The test function set the value equal to the key.
    TEST_ASSERT_EQUAL_UINT16(key, *(value_t*)value);

    // Track the number of invocations of this callback function.
    callback_8f_num_calls++;
}
void test_8f_hash_table_iterate_success(void) {
    // Pre-condition: Create a hash table.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(value_t));
    TEST_ASSERT_NOT_NULL(table);

    // Pre-condition: Insert multiple values into the hash table -- setting the value equal to the key.
    const uint16_t num_keys = 10;
    for(uint16_t key = 0; key < num_keys; key++) {
        const value_t value = key;
        const bool inserted = hash_table_insert(table, key, sizeof(value), &value, false);
        TEST_ASSERT_TRUE(inserted);
    }

    // Test: Iterate over all keys that are present in a hash table.
    value_t value = 0;
    hash_table_iterate(table, sizeof(value), &value, callback_8f);
    TEST_ASSERT_EQUAL_UINT16(num_keys, callback_8f_num_calls);

    // Cleanup: Destroy the hash table.
    hash_table_destroy(&table);
    TEST_ASSERT_NULL(table);
}
