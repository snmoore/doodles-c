#include <time.h>       // For clock
#include <stdio.h>      // For printf
#include <stdlib.h>     // For EXIT_SUCCESS
#include <stdint.h>     // For uint8_t
#include <string.h>     // For malloc

#define BYTES (1024*1024*400)

// Duff's Device in standard form
static void duff(volatile short int *to, volatile short int *from, int count)
{
    // Set some markers that can be checked later
    from[0]       = 1;
    from[count-1] = 2;
    printf("Duff's Device: [%hd %hd]\n", from[0], from[count-1]);

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

    // Check the markers to verify the code was not optimized away
    printf("Duff's Device: [%hd %hd] ", from[0], from[count-1]);

    // Check the execution time
    printf("%lu usec\n", Stop - Start);
}

static void loop(short int *to, short int *from, int count)
{
    // Set some markers that can be checked later
    from[0]       = 3;
    from[count-1] = 4;
    printf("Loop: [%hd %hd]\n", from[0], from[count-1]);

    // Copy using a simple loop, with time stamps before and after
    clock_t Start = clock();
    do
    {
        *to++ = *from++;
    } while(--count > 0);
    clock_t Stop = clock();

    // Check the markers to verify the code was not optimized away
    printf("Loop: [%hd %hd] ", from[0], from[count-1]);

    // Check the execution time
    printf("%lu usec\n", Stop - Start);
}

static uint32_t builtin(void *to, void *from, size_t bytes)
{

}

static void runner(uint32_t (*function)(void *to, void *from, size_t bytes), char* label, unsigned int iterations, uint8_t *to, uint8_t *from, size_t bytes)
{
    // Set some markers at each end that can be checked later
    static uint8_t marker = 1;
    from[0]       = marker++;
    from[bytes-1] = marker++;

    // Execute the function for the desired number of iterations
    for(unsigned int i = 0; i < iterations; i++)
    {
        printf("\n%s %d", label, i);

        /* Execute the function and measure the time taken, excluding call overhead */
        uint32_t usec = function((void *)to, (void *)from, bytes);

        /* Check the markers to ensure data was actually copied */
        if((to[0] != from[0]) || (to[bytes-1] != from[bytes-1]))
        {
            printf("data not correctly copied\n");
            break;
        }

        printf("%u usec\n", usec);
    }
}

//static void builtin(short int *to, short int *from, size_t bytes)
//{
//    // Set some markers that can be checked later
//    int end = bytes/sizeof(from[0])-1;
//    from[0] = 5;
//    from[end] = 6;
//    printf("Memcpy: [%hd %hd]\n", from[0], from[end]);
//
//    // Copy using memcpy, with time stamps before and after
//    clock_t Start = clock();
//    memcpy(to, from, bytes);
//    clock_t Stop = clock();
//
//    // Check the markers to verify the code was not optimized away
//    printf("Memcpy: [%hd %hd] ", from[0], from[end]);
//
//    // Check the execution time
//    printf("%lu usec\n", Stop - Start);
//}

int main(void)
{
    // Allocate some large source and destination buffers on the heap
    short int *Src = (short int *)malloc(BYTES);
    short int *Dst = (short int *)malloc(BYTES);
    if((Src != NULL) && (Dst != NULL))
    {
        int i = 0;

        // Run the Duff's Device test 10 times
        for(i = 0; i < 10; i++)
        {
            duff(Dst, Src, BYTES / sizeof(short int));
        }

        // Run the loop test 10 times
        for(i = 0; i < 10; i++)
        {
            loop(Dst, Src, BYTES / sizeof(short int));
        }

        // Run the memcpy test 10 times
        for(i = 0; i < 10; i++)
        {
            builtin(Dst, Src, BYTES);
        }

        // Free the memory
        free(Src);
        free(Dst);

        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}
