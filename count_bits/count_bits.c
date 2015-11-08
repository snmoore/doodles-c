// Count the number of bits set in a 32-bit word
//
// See https://graphics.stanford.edu/~seander/bithacks.html

#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS

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

int main(void) {
    unsigned int values[] = { 0, 1, 2, 3, 4, 5, 7, 8, 10, 64, 128, 255 };

    // Count the number of bits set using the obvious method
    printf("\nObvious method:\n");
    for(size_t i = 0; i < NELEMENTS(values); i++) {
        printf("Value: %3u, bits: %u\n", values[i], obvious(values[i]));
    }

    // Count the number of bits set using the obvious method
    printf("\nKernighan's method:\n");
    for(size_t i = 0; i < NELEMENTS(values); i++) {
        printf("Value: %3u, bits: %u\n", values[i], obvious(values[i]));
    }

    return EXIT_SUCCESS;
}
