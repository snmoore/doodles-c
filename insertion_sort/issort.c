#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS
#include <string.h> // For memmove

#define NELEMENTS(a)    (sizeof(a) / sizeof(a[0]))

typedef int (*compare_t)(int a, int b);

// Comparison function to compare two values
int compare_values(int a, int b) {
    // Or more simply: return a - b;
    if(a < b) {
        return -1;
    }
    else if(a > b) {
        return 1;
    }
    else {
        return 0;
    }
}

// Sort an array of values
void issort_values(int *data, size_t nelements, compare_t compare) {
    // Elements to the left of j are sorted, elements to the right are unsorted
    for(size_t j = 1; j < nelements; j++) {
        int i = (int)j - 1;

        // The value being sorted
        int value = data[j];

        // Working leftwards, find the insertion point
        while(i >= 0 && compare(data[i], value) > 0) {
            // Move the sorted elements to the right
            data[i+1] = data[i];
            i--;
        }

        // Insert the sorted value
        data[i+1] = value;
    }
}

// Sort an array of values, moving elements with memmove
void issort_values_move(int *data, size_t nelements, compare_t compare) {
    // Elements to the left of j are sorted, elements to the right are unsorted
    for(size_t j = 1; j < nelements; j++) {
        int i = (int)j - 1;

        // The value being sorted
        int value = data[j];

        // Working leftwards, find the insertion point
        while(i >= 0 && compare(data[i], value) > 0) {
            i--;
        }

        // Move the sorted elements to the right in one go
        memmove(data + i + 1, data + i, (j - i) * sizeof(int));

        // Insert the sorted value
        data[i+1] = value;
    }
}


// Print an array of values
void print(int *data, size_t nelements) {
    if(data != NULL) {
        printf("Data:\n");
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
    print(data, NELEMENTS(data));

    // Sort the data
    issort_values_move(data, NELEMENTS(data), compare_values);
    print(data, NELEMENTS(data));

    return EXIT_SUCCESS;
}
