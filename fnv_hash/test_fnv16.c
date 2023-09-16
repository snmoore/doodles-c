// Ceedling unit tests for 16-bit FNV-1a hash algorithm.
//
// See the Internet draft by Fowler, Noll, Vo, and Eastlake:
//  The FNV Non-Cryptographic Hash Algorithm
//  https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-20
//
// See https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
//
// This uses the 32-bit FNV-1a hash algorithm with XOR folding, as described in
// draft-eastlake-fnv-20, section 3. Other Hash Sizes and XOR Folding.
//
// Tests:
//  1a. Compute a 16-bit FNV-1a hash of a block of data -- null data pointer.
//  1b. Compute a 16-bit FNV-1a hash of a block of data -- zero length.
//  1c. Compute a 16-bit FNV-1a hash of a block of data -- valid data.

#include <string.h>     // For strlen
#include "unity.h"      // Unity test framework
#include "fnv16.h"      // Unit under test
#include "fnv32.h"      // Uses the 32-bit FNV-1a hash algorithm

// 16-bit FNV offset basis value ((FNV32_BASIS >> 16) ^ (FNV32_BASIS & 0xFFFF)).
#define FNV16_BASIS 0x1CD9

// Test 1a. Compute a 16-bit FNV-1a hash of a block of data -- null data pointer.
void test_1a_fnv16_fail_null_data(void) {
    uint16_t hash = fnv16(NULL, 1);
    TEST_ASSERT_EQUAL_UINT16(FNV16_BASIS, hash);
}

// Test 1b. Compute a 16-bit FNV-1a hash of a block of data -- zero length.
void test_1b_fnv16_fail_zero_length(void) {
    const uint8_t data[] = { 1 };
    uint16_t hash = fnv16(data, 0);
    TEST_ASSERT_EQUAL_UINT16(FNV16_BASIS, hash);
}

// Test 1c. Compute a 16-bit FNV-1a hash of a block of data -- valid data.
void test_1c_fnv16_success(void) {
    // Test values from draft-eastlake-fnv-20, Appendix C: A Few Test Vectors.
    typedef struct test_tag {
        const char * string;
        uint16_t     hash_excluding_null;
        uint16_t     hash_including_null;
    } test_t;

    test_t tests[] = {
        { ""       , 0x1cd9 , 0x5813 },
        { "a"      , 0xcd20 , 0xfb60 },
        { "foobar" , 0x46f4 , 0x92a4 }
    };

    // Test strings excluding null (zero byte) termination.
    for(size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        // Message to output upon failure.
        char message[100] = "";
        sprintf(message, "Failed for \"%s\" excluding null (zero byte) termination", tests[i].string);

        // Test.
        uint16_t hash = fnv16((uint8_t*)tests[i].string, strlen(tests[i].string));
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(tests[i].hash_excluding_null, hash, message);
    }

    // Test strings including null (zero byte) termination.
    for(size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        // Message to output upon failure.
        char message[100] = "";
        sprintf(message, "Failed for \"%s\" including null (zero byte) termination", tests[i].string);

        // Test.
        uint16_t hash = fnv16((uint8_t*)tests[i].string, strlen(tests[i].string) + 1);
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(tests[i].hash_including_null, hash, message);
    }
}
