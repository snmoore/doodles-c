// Count the number of bits set in a 32-bit word
//
// See https://graphics.stanford.edu/~seander/bithacks.html

#include <stdbool.h>    // For bool, true, false
#include <stdint.h>     // For uint8_t
#include <stdio.h>      // For printf
#include <stdlib.h>     // For EXIT_SUCCESS

#define NELEMENTS(a)    (sizeof(a) / sizeof(a[0]))

// Count using the obvious method; 32 iterations
unsigned int obvious(unsigned int value) {
    unsigned int count;
    for(count = 0; value > 0; value >>= 1) {
        // Increment count if the bit is set
        count += value & 1;
    }
    return count;
}

// Count using Kernighan's method; as many iterations as there are bits set
unsigned int kernighan(unsigned int value) {
    unsigned int count;
    for(count = 0; value > 0; count++) {
        // Clear the least significant bit
        value &= value - 1;
    }
    return count;
}

// Count using a lookup table
unsigned int lookup(unsigned int value) {
    // Lookup table that will contain the number of bits in all bytes 0..255
    static unsigned int bits[256]; // is initialized to all zeros

    // Only build the array once
    if(!bits[1]) {
        // Compute the number of bits set in all byte values 0..255
        for(unsigned int i = 0; i < 256; i++) {
            bits[i] = (i & 1) + bits[i/2];
        }
    }

    // Add up the number of bits from each of the 4 bytes in the 32-bit value
    return bits[ value        & 0xff] +
           bits[(value >>  8) & 0xff] +
           bits[(value >> 16) & 0xff] +
           bits[(value >> 24) & 0xff];
}

// Count bits in a byte using a compact lookup table
uint8_t compact(uint8_t value) {
    static uint8_t bits[] = {
        0,  // 0    0   0b0000
        1,  // 1    1   0b0001
        1,  // 2    2   0b0010
        2,  // 3    3   0b0011
        1,  // 4    4   0b0100
        2,  // 5    5   0b0101
        2,  // 6    6   0b0110
        3,  // 7    7   0b0111
        1,  // 8    8   0b1000
        2,  // 9    9   0b1001
        2,  // 10   A   0b1010
        3,  // 11   B   0b1011
        2,  // 12   C   0b1100
        3,  // 13   D   0b1101
        3,  // 14   E   0b1110
        4,  // 15   F   0b1111
    };
    return bits[value >> 4] + bits[value & 0x0f];
}

int main(void) {
    unsigned int values[] = { 0, 1, 2, 3, 4, 5, 7, 8, 10, 11, 12, 13, 14, 15, 64, 128, 255 };

    printf("Value  Obvious  Kernighan  Lookup  Compact\n");
    for(size_t i = 0; i < NELEMENTS(values); i++) {
        printf("%-3u    %-2u       %-2u         %-2u         %-2u\n", values[i],
               obvious(values[i]), kernighan(values[i]), lookup(values[i]), compact(values[i]));
    }

    return EXIT_SUCCESS;
}
