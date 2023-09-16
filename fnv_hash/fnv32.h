// 32-bit FNV-1a hash algorithm.
//
// See the Internet draft by Fowler, Noll, Vo, and Eastlake:
//  The FNV Non-Cryptographic Hash Algorithm
//  https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-20
//
// See https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

#ifndef FNV32_H
#define FNV32_H

#include <stdbool.h>    // For bool
#include <stddef.h>     // For size_t
#include <stdint.h>     // For uint32_t

// Compute a 32-bit FNV-1a hash of a block of data.
//
// Parameters:
//  data    : pointer to a contiguous block of data.
//  length  : length of the block of data, in bytes.
//  returns : the computed hash value (or the FNV offset basis value 0x811C9DC5 if data is null or length is 0).
uint32_t fnv32(const uint8_t * data, size_t length);

#endif
