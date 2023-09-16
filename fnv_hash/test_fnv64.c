// Ceedling unit tests for 64-bit FNV-1a hash algorithm.
//
// See the Internet draft by Fowler, Noll, Vo, and Eastlake:
//  The FNV Non-Cryptographic Hash Algorithm
//  https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-20
//
// See https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
//
// Tests:
//  1a. Compute a 64-bit FNV-1a hash of a block of data -- null data pointer.
//  1b. Compute a 64-bit FNV-1a hash of a block of data -- zero length.
//  1c. Compute a 64-bit FNV-1a hash of a block of data -- valid data.

#include <string.h>     // For strlen
#include "unity.h"      // Unity test framework
#include "fnv64.h"      // Unit under test

// 64-bit FNV offset basis value.
#define FNV64_BASIS 0xCBF29CE484222325

// Test 1a. Compute a 64-bit FNV-1a hash of a block of data -- null data pointer.
void test_1a_fnv64_fail_null_data(void) {
    uint64_t hash = fnv64(NULL, 1);
    TEST_ASSERT_EQUAL_UINT64(FNV64_BASIS, hash);
}

// Test 1b. Compute a 64-bit FNV-1a hash of a block of data -- zero length.
void test_1b_fnv64_fail_zero_length(void) {
    const uint8_t data[] = { 1 };
    uint64_t hash = fnv64(data, 0);
    TEST_ASSERT_EQUAL_UINT64(FNV64_BASIS, hash);
}

// Test 1c. Compute a 64-bit FNV-1a hash of a block of data -- valid data.
void test_1c_fnv64_success(void) {
    // Test values from draft-eastlake-fnv-20, Appendix C: A Few Test Vectors.
    typedef struct test_tag {
        const char * string;
        uint64_t     hash_excluding_null;
        uint64_t     hash_including_null;
    } test_t;

    test_t tests[] = {
        { ""       , 0xcbf29ce484222325 , 0xaf63bd4c8601b7df },
        { "a"      , 0xaf63dc4c8601ec8c , 0x089be207b544f1e4 },
        { "foobar" , 0x85944171f73967e8 , 0x34531ca7168b8f38 }
    };

    // Test strings excluding null (zero byte) termination.
    for(size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        // Message to output upon failure.
        char message[100] = "";
        sprintf(message, "Failed for \"%s\" excluding null (zero byte) termination", tests[i].string);

        // Test.
        uint64_t hash = fnv64((uint8_t*)tests[i].string, strlen(tests[i].string));
        TEST_ASSERT_EQUAL_UINT64_MESSAGE(tests[i].hash_excluding_null, hash, message);
    }

    // Test strings including null (zero byte) termination.
    for(size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        // Message to output upon failure.
        char message[100] = "";
        sprintf(message, "Failed for \"%s\" including null (zero byte) termination", tests[i].string);

        // Test.
        uint64_t hash = fnv64((uint8_t*)tests[i].string, strlen(tests[i].string) + 1);
        TEST_ASSERT_EQUAL_UINT64_MESSAGE(tests[i].hash_including_null, hash, message);
    }
}
