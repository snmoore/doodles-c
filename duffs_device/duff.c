// Duff's Device for copying data
//
// See https://en.wikipedia.org/wiki/Duff%27s_device

#include <time.h>       // For clock
#include <stdio.h>      // For printf
#include <stdlib.h>     // For EXIT_SUCCESS
#include <stdint.h>     // For uint8_t
#include <string.h>     // For malloc

#define BYTES (1024*1024*400)

// Duff's Device in standard form
static clock_t duff(volatile short int *to, volatile short int *from, size_t bytes) {
    int count = bytes/sizeof(from[0]);

    // Copy using Duff's Device, with time stamps before and after
    clock_t Start = clock();
    register int n = (count + 7) / 8;
    switch (count % 8) {
    case 0: do { *to++ = *from++;
    case 7:      *to++ = *from++;
    case 6:      *to++ = *from++;
    case 5:      *to++ = *from++;
    case 4:      *to++ = *from++;
    case 3:      *to++ = *from++;
    case 2:      *to++ = *from++;
    case 1:      *to++ = *from++;
            } while (--n > 0);
    }
    clock_t Stop = clock();

    // Return the elapsed time in microseconds
    return Stop - Start;
}

// Loop alternative to Duff's Device
static clock_t loop(volatile short int *to, volatile short int *from, size_t bytes) {
    int count = bytes/sizeof(from[0]);

    // Copy using a simple loop, with time stamps before and after
    clock_t Start = clock();
    while(count--) {
        *to++ = *from++;
    }
    clock_t Stop = clock();

    // Return the elapsed time in microseconds
    return Stop - Start;
}

// Builtin alternative to Duff's Device, using memcpy
static clock_t builtin(volatile short int *to, volatile short int *from, size_t bytes) {
    // Copy using memcpy, with time stamps before and after
    clock_t Start = clock();
    memcpy((void*)to, (void*)from, bytes);
    clock_t Stop = clock();

    // Return the elapsed time in microseconds
    return Stop - Start;
}

// Run a test multiple times using the supplied function
typedef clock_t (*function_t)(volatile short int *to, volatile short int *from, size_t bytes);
static void runner(volatile short int *dst, volatile short int *src, size_t bytes, int iterations, function_t function, char* name) {
    int end = bytes/sizeof(src[0])-1;

    // Run the test multiple times
    for(int i = 0; i < iterations; i++)
    {
        printf("%-7s src[%hd, %hd, %hd .. %hd, %hd, %hd]\n", name, src[0], src[1], src[2], src[end-2], src[end-1], src[end]);

        // Clear out the destination buffer
        memset((void*)dst, 0, BYTES);

        // Copy src to dst using the supplied function
        clock_t usec = function(dst, src, bytes);

        printf("         to[%hd, %hd, %hd .. %hd, %hd, %hd] = ", dst[0], dst[1], dst[2], dst[end-2], dst[end-1], dst[end]);
        printf("%lu usec\n", usec);

        // Verify the data was really copied
        if(memcmp((void*)dst, (void*)src, BYTES) != 0) {
            printf("!!! Data was not copied correctly\n");
        }
    }
}

int main(void) {
    // Assume failure
    int result = EXIT_FAILURE;

    // Allocate some large source and destination buffers on the heap
    short int *Src = (short int *)malloc(BYTES);
    short int *Dst = (short int *)malloc(BYTES);
    if((Src != NULL) && (Dst != NULL))
    {
        // Set the source data to be copied
        for(size_t i = 0; i < BYTES/sizeof(Src[0]); i++) {
            Src[i] = (short int)i;
        }

        // Run the Duff's Device test 10 times
        runner(Dst, Src, BYTES, 10, duff, "Duff");

        // Run the loop test 10 times
        runner(Dst, Src, BYTES, 10, loop, "Loop");

        // Run the builtin test 10 times
        runner(Dst, Src, BYTES, 10, builtin, "Builtin");

        // Success
        result = EXIT_SUCCESS;
    }

    // Free the buffers
    free(Src);
    free(Dst);

    return result;
}
