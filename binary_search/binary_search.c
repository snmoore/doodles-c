// Find the position of a target value (a key) in a sorted array using a binary
// search.
//
// Compare the target value to the value at the middle element of the set.
// If equal, return the position of the middle element. If the target value is
// smaller than the value at the middle element then continue the search in the
// lower half of the set, otherwise continue the search in the upper half of the
// set. Repeat until the target value is found or there are no more elements
// left to search, in which case the target value is not present in the set.
//
// This typically executes in log_2(N) time
//
// See https://en.wikipedia.org/wiki/Binary_search_algorithm

#include <stdio.h>      // For printf
#include <stdlib.h>     // For EXIT_SUCCESS, bsearch

#define NOT_FOUND       -1
#define NELEMENTS(a)    (sizeof(a) / sizeof(a[0]))

// Iterative binary search
int iterative(int *Values, int Key, int Lower, int Upper) {
    // Continually narrow the search until just one element remains
    while(Upper >= Lower) {
        // Calculate the midpoint to split the set in two
        int Midpoint = (Upper + Lower) / 2;
        printf("Key: %d Lower: %d Midpoint %d Upper: %d\n", Key, Lower, Midpoint, Upper);

        // Is the key at the midpoint?
        if(Key == Values[Midpoint]) {
            return Midpoint;
        }
        // Is the key within the lower half? */
        else if(Key < Values[Midpoint]) {
            // Continue the search in the lower half
            Upper = Midpoint - 1;
        }
        // Is the key within the upper half? */
        else { // if(Key > Values[Midpoint])
            // Continue the search in the upper half
            Lower = Midpoint + 1;
        }
    }

    return NOT_FOUND;
}

// Recursive binary search
int recursive(int *Values, int Key, int Lower, int Upper) {
    // Are there no more elements remaining?
    if(Upper < Lower) {
        return NOT_FOUND;
    }
    // There is at least 1 remaining element
    else {
        // Calculate the midpoint to split the set in two
        int Midpoint = (Upper + Lower) / 2;
        printf("Key: %d Lower: %d Midpoint %d Upper: %d\n", Key, Lower, Midpoint, Upper);

        // Is the key at the midpoint?
        if(Key == Values[Midpoint]) {
            return Midpoint;
        }
        // Is the key within the lower half? */
        else if(Key < Values[Midpoint]) {
            // Continue the search in the lower half
            return recursive(Values, Key, Lower, Midpoint - 1);
        }
        // Is the key within the upper half? */
        else { // if(Key > Values[Midpoint])
            // Continue the search in the upper half
            return recursive(Values, Key, Midpoint + 1, Upper);
        }
    }
}

// Recursive binary search (implementation ends with a 2 element set)
int recursive2(int *Values, int Key, int Lower, int Upper) {
    // Has the search been reduced as far as practical?
    //
    // The smallest set ideally contains 1 element, in which case the tests
    // would be Lower == Upper and Key == Values[Lower]
    //
    // But the midpoint calculation and the bisecting of the sets mean the
    // smallest set actually contains 2 elements e.g.:
    //      Iteration N:    Lower = 9, Upper = 10 ==> Midpoint = 9
    //      Iteration N+1:  Lower = 9, Upper = 10 ==> Midpoint = 9
    //
    // Hence the need to test Key == Values[Upper] as well
    if(Upper - Lower <= 1) {
        if(Key == Values[Lower]) {
            return Lower;
        }
        else if(Key == Values[Upper]) {
            return Upper;
        }
        else {
            return NOT_FOUND;
        }
    }
    // There are at least 3 remaining elements
    else {
        // The set is not empty and a match has not been found yet

        // Calculate the midpoint to split the set in two
        // Beware of simply using (Upper + Lower) / 2 because the Lower + Upper
        // addition may overflow
        int Midpoint = Lower + ((Upper - Lower) / 2);

        printf("Key: %d Lower: %d Midpoint %d Upper: %d\n", Key, Lower, Midpoint, Upper);

        // Is the key within the range of the upper set?
        if(Key >= Values[Midpoint]) {
            // Continue search in the upper set
            return recursive2(Values, Key, Midpoint, Upper);
        }
        else {
            // Continue search in the lower set
            return recursive2(Values, Key, Lower, Midpoint - 1);
        }
    }
}

// Comparison function used with bsearch
int Compare(const void *Key, const void *Value) {
    int key   = *(int *) Key;
    int value = *(int *) Value;

    printf("Key: %d Value: %d\n", key, value);

    if(key < value) {
        return -1;
    }
    else if(key > value) {
        return 1;
    }
    else { // key == value
        return 0;
    }
}

// Binary search using the built-in function from the C library
int builtin(int *Values, size_t NumElements, int *Key) {
    int *Match = (int *)bsearch(Key, Values, NumElements, sizeof(Values[0]), Compare);
    if(Match != NULL) {
        return Match - Values;
    }
    else {
        return NOT_FOUND;
    }
}

int main(void) {
    int Values[] = { 1, 2, 3, 4, 5, 7, 8, 9, 11, 13, 16, 17, 23, 27, 29, 32, 31, 37, 64, 81 };
    int Key      = NOT_FOUND;
    int Index    = NOT_FOUND;

    Key = 3;
    Index = iterative(Values, Key, 0, NELEMENTS(Values) - 1);
    printf("Iterative: %d ==> %d\n\n", Key, Index);

    Key = 14;
    Index = iterative(Values, Key, 0, NELEMENTS(Values) - 1);
    printf("Iterative: %d ==> %d\n\n", Key, Index);

    Key = 3;
    Index = recursive(Values, Key, 0, NELEMENTS(Values) - 1);
    printf("Recursive: %d ==> %d\n\n", Key, Index);

    Key = 14;
    Index = recursive(Values, Key, 0, NELEMENTS(Values) - 1);
    printf("Recursive: %d ==> %d\n\n", Key, Index);

    Key = 3;
    Index = recursive2(Values, Key, 0, NELEMENTS(Values) - 1);
    printf("Recursive 2: %d ==> %d\n\n", Key, Index);

    Key = 14;
    Index = recursive2(Values, Key, 0, NELEMENTS(Values) - 1);
    printf("Recursive 2: %d ==> %d\n\n", Key, Index);

    Key = 3;
    Index = builtin(Values, NELEMENTS(Values), &Key);
    printf("Built-in: %d ==> %d\n\n", Key, Index);

    Key = 14;
    Index = builtin(Values, NELEMENTS(Values), &Key);
    printf("Built-in: %d ==> %d\n\n", Key, Index);

    return EXIT_SUCCESS;
}
