// 16-bit FNV-1a hash algorithm.
//
// See the Internet draft by Fowler, Noll, Vo, and Eastlake:
//  The FNV Non-Cryptographic Hash Algorithm
//  https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-17
//
// See https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
//
// This uses the 32-bit FNV-1a hash algorithm with XOR folding, as described in
// draft-eastlake-fnv-20, section 3. Other Hash Sizes and XOR Folding.

#include "fnv16.h"
#include "fnv32.h"

// Compute a 16-bit FNV-1a hash of a block of data.
//
// Parameters:
//  data    : pointer to a contiguous block of data.
//  length  : length of the block of data, in bytes.
//  returns : the computed hash value (or the FNV offset basis value 0x1CD9 if data is null or length is 0).
uint16_t fnv16(const uint8_t * data, size_t length) {
    // Compute the 32-bit FNV-1a hash.
    const uint32_t hash32 = fnv32(data, length);

    // XOR fold to 16-bits.
    return (uint16_t)((hash32 >> 16) ^ (hash32 & 0xFFFF));
}
