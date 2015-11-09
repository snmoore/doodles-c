// Generate the first 13 terms of the Fibonacci sequence:
//      0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, ...
//
// In mathematical terms, the sequence Fn of Fibonacci numbers is defined by the recurrence relation
//      F{n} = F{n-1} + F{n-2}
//  with seed values
//      F{1} = 1, F{2} = 1
//  or (modern usage)
//      F{0} = 0, F{1} = 1
//
// See https://en.wikipedia.org/wiki/Fibonacci_number

#include <stdio.h>
#include <stdlib.h>

// Iteratively print the Fibonacci sequence
static void iterative(unsigned int n) {
    unsigned int Fn_2 = 0; // F{0} = 0
    unsigned int Fn_1 = 1; // F{1} = 1
    unsigned int Fn   = 0;
    unsigned int i    = 0;

    printf("Iterative:\n%u %u ", Fn_2, Fn_1);
    for(i = 2; i < n; i++) {
        Fn   = Fn_1 + Fn_2;
        Fn_2 = Fn_1;
        Fn_1 = Fn;
        printf("%u ", Fn);
    }
    printf("\n\n");
}

// Value of the nth term of the Fibonacci sequence, using recursion
static unsigned int recurse(unsigned int n) {
    if(n <= 1) {
        return n;
    }
    else {
        return recurse(n - 1) + recurse(n - 2);
    }
}

// Recursively print the Fibonacci sequence
static void recursive(unsigned int n) {
    unsigned int i = 0;

    printf("Recursive:\n");
    for(i = 0; i < n; i++) {
        printf("%u ", recurse(i));
    }
    printf("\n\n");
}

int main(void) {
    iterative(13);
    recursive(13);

    return EXIT_SUCCESS;
}
