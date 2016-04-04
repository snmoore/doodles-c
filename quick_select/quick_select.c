// Find the median of an array of values using quickselect
//
// This works like quicksort, but only recurses into one side – the side with
// the element it is searching for - and it can stop as soon as the partition
// reaches the median position
//
// This has an average case performance of O(n)
//
// See https://en.wikipedia.org/wiki/Quickselect

#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS, NULL

#define NELEMENTS(a)    (sizeof(a)/sizeof(a[0]))

// Print an array of values
void print(const char* msg, const int *data, size_t nelements) {
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

// Find the median in an array of values using quickselect
int quickselect(int* data, size_t nelements, size_t lo, size_t hi) {
    if(lo < hi) {
        // Select the rightmost element as the pivot
        int pivot = data[hi];

        // Partition using Lomuto partitioning
        size_t partition = lo;
        for(size_t i = lo; i < hi; i++) {
            // Move smaller elements to the left
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

        print("During:        ", data, nelements);

        // Is the partition at the mid-point?
        if(partition == nelements/2) {
            return data[nelements/2];
        }
        // Is the partition to the right of the mid-point?
        else if(partition > nelements/2) {
            // Recurse into the left partition
            return quickselect(data, nelements, lo, partition - 1);
        }
        // Is the partition to the left of the mid-point?
        else {
            // Recurse into the right partition
            return quickselect(data, nelements, partition + 1, hi);
        }
    }
    else {
        // Only one element left
        return data[lo];
    }
}

int main(void) {
    // Create an array of unsorted data
    int data[] = { 23, 21, 76, 16, 43, 52, 18 };
    print("Unsorted:      ", data, NELEMENTS(data));

    // Find the median using the quick select algorithm
    int median = quickselect(data, NELEMENTS(data), 0, NELEMENTS(data) - 1);
    print("Partly sorted: ", data, NELEMENTS(data));
    printf("Median value:  %d\n", median);

    return EXIT_SUCCESS;
}
