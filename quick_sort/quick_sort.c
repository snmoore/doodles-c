// Sort an array of values using quicksort
//
// This has an average case performance of O(n log n)
//
// See https://en.wikipedia.org/wiki/Quicksort

#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS, NULL

#define NELEMENTS(a)    (sizeof(a)/sizeof(a[0]))

// Sort an array of values using quicksort
void quicksort(int* data, size_t lo, size_t hi) {
    if(lo < hi) {
        // Select the rightmost element as the pivot
        int pivot = data[hi];

        // Partition using Lomuto partitioning
        size_t partition = lo;
        for(size_t i = lo; i < hi; i++) {
            if(data[i] <= pivot) {
                // Swap the element with the partition
                int temp        = data[i];
                data[i]         = data[partition];
                data[partition] = temp;

                // Move the partition rightwards
                partition++;
            }
        }
        // Swap the pivot with the partition
        data[hi]        = data[partition];
        data[partition] = pivot;

        // Recurse into the left sub-array
        if(partition > 0) { // beware of underflow
            quicksort(data, lo, partition - 1);
        }

        // Recurse into the right sub-array
        quicksort(data, partition + 1, hi);
    }
}

// Print an array of values
void print(const char* msg, int *data, size_t nelements) {
    if(data != NULL) {
        printf("%s", msg);
        for(size_t i = 0; i < nelements; i++) {
            printf("%2d ", data[i]);
        }
        printf("\n");
    }
    else {
        printf("Bad arguments!\n");
    }
}

int main(void) {
    // Create an array of unsorted data
    int data[] = { 23, 21, 76, 16, 52, 43 };
    print("Unsorted: ", data, NELEMENTS(data));

    // Sort the data
    quicksort(data, 0, NELEMENTS(data) - 1);
    print("Sorted:   ", data, NELEMENTS(data));

    return EXIT_SUCCESS;
}
