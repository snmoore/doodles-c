// 32-bit and 64-bit FNV-1a hash algorithms.
//
// See the Internet draft by Fowler, Noll, Vo, and Eastlake:
//  The FNV Non-Cryptographic Hash Algorithm
//  https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-17
//
// See https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
//
// Example:
//
//  ./fnv_hash Hash some strings
//
// Returns:
//
// String  | 32-bit FNV-1a | 64-bit FNV-1a
// Hash    | 0x4ef356f1    | 0x58cb9fd8758aebf1
// some    | 0xf3611c71    | 0x6035dc18f0bbd4d1
// strings | 0xb0727511    | 0x80eb3bb1f9097d11

#include <stddef.h>     /* For size_t */
#include <stdio.h>      /* For printf */
#include <stdlib.h>     /* For EXIT_FAILURE, EXIT_SUCCESS */
#include <string.h>     /* For strlen */
#include "fnv32.h"      /* For fnv32 */
#include "fnv64.h"      /* For fnv64 */

int main(int argc, char *argv[]) {
    // Process the command line
    if(argc < 2) {
        printf("Usage: ./fnv_hash STRINGS\n");
        return EXIT_FAILURE;
    }

    // Find the length of the longest string, used later for formatting the output
    size_t max_length = strlen("String");
    for(int i = 1; i < argc; i++) {
        const size_t length = strlen(argv[i]);
        if(length > max_length) {
            max_length = length;
        }
    }

    // Output a header
    printf("%-*s | %s | %s\n", (int)max_length, "String", "32-bit FNV-1a", "64-bit FNV-1a");

    // Compute and output the 32-bit and 64-bit FNV-1a hash of each supplied string
    for(int i = 1; i < argc; i++) {
        const size_t   length = strlen(argv[i]);
        const uint32_t hash32 = fnv32((uint8_t*)argv[i], length);
        const uint64_t hash64 = fnv64((uint8_t*)argv[i], length);
        printf("%-*s | 0x%08x    | 0x%016llx\n", (int)max_length, argv[i], hash32, hash64);
    }

    return EXIT_SUCCESS;
}
