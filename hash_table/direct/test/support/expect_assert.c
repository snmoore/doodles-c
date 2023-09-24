// Ceedling test support for expecting assert() failures.

#include <stdbool.h>    // For bool
#include <stdio.h>      // For sprintf
#include "unity.h"      // Unity test framework

// Flag to control the expect.
static bool expected = false;

// Expect an assert() failure.
void expect_assert(void) {
    expected = true;
}

// Clear the expect for an assert() failure.
void expect_assert_clear(void) {
    expected = false;
}

// Platform independent stub for assert() failures.
static void stub_assert(const char * function, const char * assertion) {
    if(expected) {
        // Abort the test immediately with a PASS state, ignoring the remainder of the test.
        TEST_PASS();
    }
    else {
        // Abort the test immediately with a FAIL state, ignoring the remainder of the test.
        char message[100];
        sprintf(message, "Assertion failed in %s: %s", function, assertion);
        TEST_FAIL_MESSAGE(message);
    }
}

// Platform dependent stubs for assert() failures.
#if defined(__linux__)
void __assert_fail(const char * assertion, const char * file, unsigned int line, const char * function) {
    (void)file;
    (void)line;
    stub_assert(function, assertion);
}
#elif defined(__APPLE__)
void __assert_rtn(const char * function, const char * file, int line, const char * assertion) {
    (void)file;
    (void)line;
    stub_assert(function, assertion);
}
#endif
