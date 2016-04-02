// Trivial byte swapping for little- vs. big-endian

#include <stdio.h>      // For printf
#include <stdint.h>     // For uint32_t
#include <stdlib.h>     // For EXIT_SUCCESS

// Swap bytes i.e. for converting between little- and big-endian
uint32_t swap(uint32_t value) {
    uint32_t result = 0;

    result |= (value & 0x000000FF) << 24;
    result |= (value & 0x0000FF00) << 8;
    result |= (value & 0x00FF0000) >> 8;
    result |= (value & 0xFF000000) >> 24;

    return result;
}

uint32_t swap2(uint32_t value) {
    uint32_t result = 0;

    result |= (value         & 0x000000FF) << 24;
    result |= ((value >>  8) & 0x000000FF) << 16;
    result |= ((value >> 16) & 0x000000FF) <<  8;
    result |= ((value >> 24) & 0x000000FF);

    return result;
}

// Determine if the host platform is little- or big-endian
void endian(void) {
    static const uint32_t value = 0x0a0b0c0d;

    if(*(const char*)&value == 0x0d) {
        printf("Little endian\n");
    }
    else {
        printf("Big endian\n");
    }
}

int main(void) {
    endian();

    printf("\n");

    uint32_t value = 0x0a0b0c0d;
    uint32_t swapped = swap(value);
    printf("%u : %u\n", value, swapped);
    printf("0x%x : 0x%x\n", value, swapped);

    printf("\n");

    uint32_t value2 = 0x0a0b0c0d;
    uint32_t swapped2 = swap(value2);
    printf("%u : %u\n", value2, swapped2);
    printf("0x%x : 0x%x\n", value2, swapped2);

    return EXIT_SUCCESS;
}
