// Ceedling unit tests for Fowler-Noll-Vo (FNV) hash algorithms.
//
// See the Internet draft by Fowler, Noll, Vo, and Eastlake:
//  The FNV Non-Cryptographic Hash Algorithm
//  https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-17
//
// See https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
//
// Tests:
//  1a. Compute a 32-bit FNV-1a hash of a block of data -- null data pointer.
//  1b. Compute a 32-bit FNV-1a hash of a block of data -- zero length.
//  1c. Compute a 32-bit FNV-1a hash of a block of data -- valid data.

#include <string.h>     // For strlen
#include "unity.h"      // Unity test framework
#include "fnv.h"        // Unit under test

// FNV offset basis values
#define FNV32_BASIS 0x811C9DC5

// Test 1a. Compute a 32-bit FNV-1a hash of a block of data -- null data pointer.
void test_1a_fnv32_fail_null_data(void) {
    uint32_t hash = fnv32(NULL, 1);
    TEST_ASSERT_EQUAL_UINT32(FNV32_BASIS, hash);
}

// Test 1b. Compute a 32-bit FNV-1a hash of a block of data -- zero length.
void test_1b_fnv32_fail_zero_length(void) {
    const uint8_t data[] = { 1 };
    uint32_t hash = fnv32(data, 0);
    TEST_ASSERT_EQUAL_UINT32(FNV32_BASIS, hash);
}

// Test 1c. Compute a 32-bit FNV-1a hash of a block of data -- valid data.
void test_1c_fnv32_fail_success(void) {
    // Test values from draft-eastlake-fnv-17, Appendix C: A Few Test Vectors
    typedef struct test_tag {
        const char * string;
        uint32_t     hash_excluding_null;
        uint32_t     hash_including_null;
    } test_t;

    test_t tests[] = {
        { ""       , 0x811c9dc5 , 0x050c5d1f },
        { "a"      , 0xe40c292c , 0x2b24d044 },
        { "foobar" , 0xbf9cf968 , 0x0c1c9eb8 }
    };

    // Test strings excluding null (zero byte) termination
    for(size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        // Message to output upon failure.
        char message[100] = "";
        sprintf(message, "Failed for \"%s\" excluding null (zero byte) termination", tests[i].string);

        // Test
        uint32_t hash = fnv32((uint8_t*)tests[i].string, strlen(tests[i].string));
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(tests[i].hash_excluding_null, hash, message);
    }

    // Test strings including null (zero byte) termination
    for(size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        // Message to output upon failure.
        char message[100] = "";
        sprintf(message, "Failed for \"%s\" including null (zero byte) termination", tests[i].string);

        // Test
        uint32_t hash = fnv32((uint8_t*)tests[i].string, strlen(tests[i].string) + 1);
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(tests[i].hash_including_null, hash, message);
    }
}
