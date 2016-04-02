// Sort an array of values using insertion sort
//
// This has an average case performance of O(n^2)
//
// See https://en.wikipedia.org/wiki/Insertion_sort

#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS
#include <string.h> // For memmove

#define NELEMENTS(a)    (sizeof(a) / sizeof(a[0]))

// Comparison function used when sorting
int compare(int a, int b) {
    return a - b;
}

// Insertion sort moving one element at a time
void insertion_sort(int* data, size_t nelements, int (*compare)(int a, int b)) {
    if((data == NULL) || (nelements == 0) || (compare == NULL)) {
        printf("Bad arguments\n");
        return;
    }

    // Move rightwards through the elements, keeping sorted elements on the left
    for(size_t j = 1; j < nelements; j++) {
        // The element being sorted will be overwritten
        int value = data[j];

        // Working leftwards, find the insertion point
        int i = (int)j - 1;
        while((i >= 0) && (compare(value, data[i]) < 0)) {
            // Move the larger element to the right
            data[i+1] = data[i];
            i--;
        }

        // Move the sorted element to the left
        data[i+1] = value;
    }
}

// Insertion sort moving elements as a block
void insertion_sort_move(int* data, size_t nelements, int (*compare)(int a, int b)) {
    if((data == NULL) || (nelements == 0) || (compare == NULL)) {
        printf("Bad arguments\n");
        return;
    }

    // Move rightwards through the elements, keeping sorted elements on the left
    for(size_t j = 1; j < nelements; j++) {
        // The element being sorted will be overwritten
        int value = data[j];

        // Working leftwards, find the insertion point
        int i = (int)j - 1;
        while((i >= 0) && (compare(value, data[i]) < 0)) {
            i--;
        }

        // Move the larger elements to the right as a block
        memmove(data + i + 1, data + i, (j - i) * sizeof(int));

        // Move the sorted element to the left
        data[i+1] = value;
    }
}

// Print an array of values
void print(const char* msg, const int *data, size_t nelements) {
    if((data == NULL) || (nelements == 0)) {
        printf("Bad arguments\n");
        return;
    }

    printf("%s", msg);
    for(size_t i = 0; i < nelements; i++) {
        printf("%2d ", data[i]);
    }
    printf("\n");
}

int main(void) {
    // Insertion sort moving one element at a time
    printf("Insertion sort moving one element at a time:\n");
    int data[] = { 23, 21, 76, 16, 52, 43 };
    print("Unsorted: ", data, NELEMENTS(data));
    insertion_sort(data, NELEMENTS(data), compare);
    print("Sorted:   ", data, NELEMENTS(data));

    printf("\n");

    // Insertion sort moving elements as a block
    printf("Insertion sort moving elements as a block:\n");
    int data2[] = { 96, 54, 57, 4, 76, 85 };
    print("Unsorted: ", data2, NELEMENTS(data2));
    insertion_sort_move(data2, NELEMENTS(data2), compare);
    print("Sorted:   ", data2, NELEMENTS(data2));

    return EXIT_SUCCESS;
}
