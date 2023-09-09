// 64-bit FNV-1a hash algorithm.
//
// See the Internet draft by Fowler, Noll, Vo, and Eastlake:
//  The FNV Non-Cryptographic Hash Algorithm
//  https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-17
//
// See https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

#include "fnv64.h"

// Compute a 64-bit FNV-1a hash of a block of data.
//
// Parameters:
//  data    : pointer to a contiguous block of data.
//  length  : length of the block of data, in bytes.
//  returns : the computed hash value (or the FNV offset basis value 0xCBF29CE484222325 if data is null or length is 0).
uint64_t fnv64(const uint8_t * data, size_t length) {
#define FNV64_PRIME 0x00000100000001B3 // 2^40 + 2^8 + 0xb3
#define FNV64_BASIS 0xCBF29CE484222325

    if((data == NULL) || (length == 0)) {
        return FNV64_BASIS;
    }

    uint64_t hash = FNV64_BASIS;
    for(size_t i = 0; i < length; i++) {
        hash ^= data[i];
        hash *= FNV64_PRIME;
    }
    return hash;
}
