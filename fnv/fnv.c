// Fowler-Noll-Vo (FNV) hash algorithms.
//
// See the Internet draft by Fowler, Noll, Vo, and Eastlake:
//  The FNV Non-Cryptographic Hash Algorithm
//  https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-17
//
// See https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

#include "fnv.h"

// Compute a 32-bit FNV-1a hash of a block of data.
//
// Parameters:
//  data    : pointer to a contiguous block of data.
//  length  : length of the block of data, in bytes.
//  returns : the computed hash value (or the FNV offset basis value 0x811c9dc5 if data is null or length is 0).
uint32_t fnv32(const uint8_t * data, size_t length) {
#define FNV32_PRIME 0x01000193 // 2^24 + 2^8 + 0x93
#define FNV32_BASIS 0x811C9DC5

    if((data == NULL) || (length == 0)) {
        return FNV32_BASIS;
    }

    uint32_t hash = FNV32_BASIS;
    for(size_t i = 0; i < length; i++) {
        hash ^= data[i];
        hash *= FNV32_PRIME;
    }
    return hash;
}
