// A circular buffer (or ring buffer)

#include <stdbool.h>    // For bool, true, false
#include <stdio.h>      // For printf
#include <stdlib.h>     // For calloc, NULL, EXIT_FAILURE, EXIT_SUCCESS, size_t
#include <string.h>     // For memcpy, memset

// A circular buffer storing chars
typedef struct circular_t {
    size_t capacity;    // capacity of the buffer i.e. total number of elements
    size_t occupied;    // number of elements currently occupied
    size_t head;        // index from which to read
    size_t tail;        // index at which to write
    char*  buffer;      // buffer allocated on the heap
} circular_t;

// Create a new circular buffer
circular_t* create(size_t capacity) {
    if(capacity == 0) {
        printf("Bad argument\n");
        return NULL;
    }

    // Allocate memory for the circular buffer
    circular_t* circular = malloc(sizeof(circular_t));
    if(circular == NULL) {
        printf("Failed to allocate struct\n");
        return NULL;
    }
    circular->buffer = calloc(capacity, sizeof(char));
    if(circular == NULL) {
        printf("Failed to allocate buffer\n");
        free(circular);
        return NULL;
    }

    // Initialize the parameters
    circular->capacity = capacity;
    circular->occupied = 0;
    circular->head     = 0;
    circular->tail     = 0;

    return circular;
}

// Destroy a circular buffer
void destroy(circular_t** circular) {
    if((circular == NULL) || (*circular == NULL)) {
        printf("Bad circular buffer\n");
        return;
    }

    free((*circular)->buffer);
    free(*circular);
    circular = NULL;
}

// Read a single element from the head
char read(circular_t* circular) {
    if(circular == NULL) {
        printf("Bad circular buffer\n");
        return 0;
    }

    if(circular->occupied > 0) {
        // Read the element
        char c = circular->buffer[circular->head];

        // Clear the element
        circular->buffer[circular->head] = ' ';

        // Update the parameters
        circular->occupied--;
        circular->head++;
        circular->head %= circular->capacity;

        return c;
    }
    else {
        printf("Buffer is empty\n");
        return 0;
    }
}

// Read many at a time from the head
size_t read_many(circular_t* circular, char* data, size_t nelements) {
    if((circular == NULL) || (data == NULL) || (nelements == 0)) {
        printf("Bad arguments\n");
        return 0;
    }

    if(nelements > circular->occupied) {
        printf("Truncating\n");
        nelements = circular->occupied;
    }

    if(nelements > 0) {
        // [..H..T..]
        if(circular->head < circular->tail) {
            memcpy(data, circular->buffer + circular->head, nelements * sizeof(data[0]));
            memset(circular->buffer + circular->head, ' ', nelements * sizeof(data[0]));
        }
        // [..T..H..] without wrap-around
        else if((circular->capacity - circular->head) > nelements) {
            memcpy(data, circular->buffer + circular->head, nelements * sizeof(data[0]));
            memset(circular->buffer + circular->head, ' ', nelements * sizeof(data[0]));
        }
        // [..T..H..] with wrap-around
        else {
            // Elements to the right of the head
            size_t relements = circular->capacity - circular->head;
            memcpy(data, circular->buffer + circular->head, relements * sizeof(data[0]));
            memset(circular->buffer + circular->head, ' ', relements * sizeof(data[0]));

            // Elements to the left of the tail
            size_t lelements = nelements - relements;
            memcpy(data + relements, circular->buffer, lelements * sizeof(data[0]));
            memset(circular->buffer, ' ', lelements * sizeof(data[0]));
        }

        // Update the parameters
        circular->occupied -= nelements;
        circular->head += nelements;
        circular->head %= circular->capacity;
    }
    else {
        printf("Buffer is empty\n");
    }

    // Return the number of elements actually read
    return nelements;
}

// Write a single element to the tail
void write(circular_t* circular, char c) {
    if(circular == NULL) {
        printf("Bad circular buffer\n");
        return;
    }

    if((circular->capacity - circular->occupied) > 0) {
        // Write the element
        circular->buffer[circular->tail] = c;

        // Update the parameters
        circular->occupied++;
        circular->tail++;
        circular->tail %= circular->capacity;
    }
    else {
        printf("Buffer is full\n");
    }
}

// Write many at a time to the tail
size_t write_many(circular_t* circular, const char* data, size_t nelements) {
    if((circular == NULL) || (data == NULL) || (nelements == 0)) {
        printf("Bad arguments\n");
        return 0;
    }

    if(nelements > (circular->capacity - circular->occupied)) {
        printf("Truncating\n");
        nelements = (circular->capacity - circular->occupied);
    }

    if(nelements > 0) {
        // [..T..H..]
        if(circular->tail < circular->head) {
            memcpy(circular->buffer + circular->tail, data, nelements * sizeof(data[0]));
        }
        // [..H..T..] without wrap-around
        else if((circular->capacity - circular->tail) > nelements) {
            memcpy(circular->buffer + circular->tail, data, nelements * sizeof(data[0]));
        }
        // [..H..T..] with wrap-around
        else {
            // Elements to the right of the tail
            size_t relements = circular->capacity - circular->tail;
            memcpy(circular->buffer + circular->tail, data, relements * sizeof(data[0]));

            // Elements to the left of the head
            size_t lelements = nelements - relements;
            memcpy(circular->buffer, data + relements, lelements * sizeof(data[0]));
        }

        // Update the parameters
        circular->occupied += nelements;
        circular->tail     += nelements;
        circular->tail     %= circular->capacity;
    }
    else {
        printf("Buffer is full\n");
    }

    // Return the number of elements actually written
    return nelements;
}

// Print a representation of a circular buffer
void print(circular_t* circular) {
    if(circular == NULL) {
        printf("Bad circular buffer\n");
        return;
    }

    printf("Capacity: %2lu\n", circular->capacity);
    printf("Occupied: %2lu\n", circular->occupied);
    printf("Head:     %2lu\n", circular->head);
    printf("Tail:     %2lu\n", circular->tail);

    printf("Contents:\n");
    for(size_t i = 0; i < circular->capacity; i++) {
        printf("%2lu ", i);
    }
    printf("\n");
    for(size_t i = 0; i < circular->capacity; i++) {
        printf("%2c ", circular->buffer[i]);
    }
    printf("\n");
    for(size_t i = 0; i < circular->capacity; i++) {
        if(circular->head == i) {
            printf("%*s", (int)i*3+2, "H");
        }
    }
    printf("\n");
    for(size_t i = 0; i < circular->capacity; i++) {
        if(circular->tail == i) {
            printf("%*s", (int)i*3+2, "T");
        }
    }
    printf("\n");
}

int main(void) {
    // Create a circular buffer
    printf("Create a circular buffer:\n");
    circular_t* circular = create(20);
    if(circular == NULL) {
        return EXIT_FAILURE;
    }
    print(circular);

    // Read when the buffer is empty
    printf("\nRead when the buffer is empty: ");
    char c = read(circular);
    if(c != 0) {
        printf("Bad read\n");
    }
    print(circular);

    // Write one at a time to the tail
    printf("\nWrite one at a time to the tail: ");
    for(int i = 0; i < 10; i++) {
        char c = 'a' + i;
        write(circular, c);
        printf("%c ", c);
    }
    printf("\n");
    print(circular);

    // Read one at a time from the head
    printf("\nRead one at a time from the head: ");
    for(int i = 0; i < 5; i++) {
        char c = read(circular);
        printf("%c ", c);
    }
    printf("\n");
    print(circular);

    // Write one at a time to the tail and wrap-around
    printf("\nWrite one at a time to the tail and wrap-around: ");
    for(int i = 10; i < 24; i++) {
        char c = 'a' + i;
        write(circular, c);
        printf("%c ", c);
    }
    printf("\n");
    print(circular);

    // Read one at a time from the head and wrap-around
    printf("\nRead one at a time from the head and wrap-around: ");
    for(int i = 0; i < 17; i++) {
        char c = read(circular);
        printf("%c ", c);
    }
    printf("\n");
    print(circular);

    // Write one at a time to the tail and hit the head
    printf("\nWrite one at a time to the tail and hit the head: ");
    for(int i = 0; i < 20; i++) {
        char c = 'a' + i;
        write(circular, c);
        printf("%c ", c);
    }
    printf("\n");
    print(circular);

    // Read one at a time from the head and hit the tail
    printf("\nRead one at a time from the head and hit the tail: ");
    for(int i = 0; i < 22; i++) {
        char c = read(circular);
        printf("%c ", c);
    }
    printf("\n");
    print(circular);

    // Start anew
    destroy(&circular);
    printf("\nStart anew:\n");
    circular = create(20);
    if(circular == NULL) {
        return EXIT_FAILURE;
    }
    print(circular);

    // Write many at a time to the tail
    printf("\nWrite many at a time to the tail: ");
    const char* data = "abcdefghijklmno";
    size_t count = write_many(circular, data, strlen(data));
    printf("%s (%zu elements)\n", data, count);
    print(circular);

    // Read many at a time from the head
    printf("\nRead many at a time from the head: ");
    char buffer[11];
    count = read_many(circular, buffer, 10);
    buffer[10] = '\0';
    printf("%s (%zu elements)\n", buffer, count);
    print(circular);

    // Write many at a time to the tail and wrap-around
    printf("\nWrite many at a time to the tail and wrap-around: ");
    const char* data2 = "pqrstuvxwyz";
    count = write_many(circular, data2, strlen(data2));
    printf("%s (%zu elements)\n", data2, count);
    print(circular);

    // Read many at a time from the head and wrap-around
    printf("\nRead many at a time from the head and wrap-around: ");
    char buffer2[15];
    count = read_many(circular, buffer2, 14);
    buffer2[14] = '\0';
    printf("%s (%zu elements)\n", buffer2, count);
    print(circular);

    // Write many at a time to the tail and hit the head
    printf("\nWrite many at a time to the tail and hit the head: ");
    const char* data3 = "abcdefghijklmnopqrstuvwxyz";
    count = write_many(circular, data3, strlen(data3));
    printf("%s (%zu elements)\n", data3, count);
    print(circular);

    // Read many at a time from the head and hit the tail
    printf("\nRead many at a time from the head and hit the tail: ");
    char buffer3[27];
    count = read_many(circular, buffer3, 26);
    buffer3[26] = '\0';
    printf("%s (%zu elements)\n", buffer3, count);
    print(circular);

    // Clean-up
    destroy(&circular);
    return EXIT_SUCCESS;
}
