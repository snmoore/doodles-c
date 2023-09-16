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

#ifndef FNV16_H
#define FNV16_H

#include <stdbool.h>    // For bool
#include <stddef.h>     // For size_t
#include <stdint.h>     // For uint16_t

// Compute a 16-bit FNV-1a hash of a block of data.
//
// Parameters:
//  data    : pointer to a contiguous block of data.
//  length  : length of the block of data, in bytes.
//  returns : the computed hash value (or the FNV offset basis value 0x1CD9 if data is null or length is 0).
uint16_t fnv16(const uint8_t * data, size_t length);

#endif
