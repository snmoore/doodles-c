// Round up an integer to the next highest power of 2

#include <limits.h> // For UINT_MAX;
#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS

// Round up an integer to the next highest power of 2 using the classic but non-obvious method
unsigned int roundup(unsigned int v) {
    // 0 should round up to 1
    v += (v == 0);

    // The classic method
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

// Round up an integer to the next highest power of 2 using a slower but more obvious method
unsigned int obvious(unsigned int v) {
    // Is v a power of 2?
    if((v & (v - 1)) == 0) {
        // 0 should round up to 1
        v += (v == 0);
        return v;
    }
    else
    {
        // Avoid overflow
        if(v == UINT_MAX) {
            return 0;
        } else {
            // Next highest power is represented by setting the bit to the left of the MSB
            unsigned int bit = 1;
            while(v >>= 1) {
                bit++;
            }
            return 1 << bit;
        }
    }
}

int main(void) {
    printf("Round up to the next highest power of 2:\n");
    for(unsigned int i = 0; i < 11; i++) {
        printf("%2u: %2u %2u\n", i, roundup(i), obvious(i));
    }
    printf("%2u: %2u %2u\n", UINT_MAX, roundup(UINT_MAX), obvious(UINT_MAX));

    return EXIT_SUCCESS;
}
