// A circular buffer (or ring buffer)

#include <stdbool.h>    // For bool, true, false
#include <stdio.h>      // For printf
#include <stdlib.h>     // For malloc, NULL, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>     // For memcpy

typedef struct circular_t {
    int head;
    int tail;
    int capacity;
    char* buffer;
} circular_t;

// Create a circular buffer
circular_t* create(unsigned int nelements) {
    // Allocate the structure
    circular_t* circular = malloc(sizeof(circular_t));
    if(circular == NULL) {
        return NULL;
    }

    // Allocate the buffer
    circular->buffer = malloc(nelements * sizeof(char));
    if(circular->buffer == NULL) {
        free(circular);
        return NULL;
    }

    circular->head = 0;
    circular->tail = 0;
    circular->capacity = nelements;

    return circular;
}

// Destroy a circular buffer
void destroy(circular_t** circular) {
    if(circular != NULL) {
        // Free the buffer
        free((*circular)->buffer);

        // Free the structure
        free(*circular);
    }
}

// How many free elements are available in the buffer?
int available(circular_t* circular) {
    if(circular == NULL) {
        return 0;
    }

    // If the buffer is completely empty
    if(circular->head == circular->tail) {
        return circular->capacity;
    }
    // If the buffer is partly full, and is not wrapped around
    else if(circular->head < circular->tail) {
        return circular->capacity - (circular->tail - circular->head);
    }
    // If the buffer is partly full, and is wrapped around
    else {
        return circular->head - circular->tail;
    }
}

// Write some data to the tail of a circular buffer
bool write(circular_t* circular, char* data, int nelements) {
    if(circular == NULL) {
        printf("Bad argument\n");
        return false;
    }

    if(nelements > available(circular)) {
        printf("Not enough free elements\n");
        return false;
    }

    printf("Appending %d elements: %s\n", nelements, data);

    // Append without wrap-around
    if(circular->capacity - circular->tail > nelements) {
        memcpy(circular->buffer + circular->tail, data, nelements * sizeof(char));
        circular->tail += nelements;
    }
    // Append with wrap-around
    else {
        // Elements at the right side of the buffer
        int right = circular->capacity - circular->tail;
        memcpy(circular->buffer + circular->tail, data, right * sizeof(char));

        // Elements at the left side of the buffer
        int left = nelements - right;
        memcpy(circular->buffer, data+right, left * sizeof(char));

        circular->tail += nelements;
        circular->tail %= circular->capacity;   // can just use -= for efficiency
    }

    return true;
}

// Read some elements from the head of a circular buffer
bool read(circular_t* circular, int nelements) {
    if(circular == NULL) {
        printf("Bad argument\n");
        return false;
    }

    if(nelements > circular->capacity - available(circular)) {
        printf("Not enough elements to read\n");
        return false;
    }

    // Remove without wrap-around
    printf("Remove %d elements: %.*s\n", nelements, nelements, circular->buffer + circular->head);
    if(circular->capacity - circular->head > nelements) {
        memset(circular->buffer + circular->head, ' ', nelements * sizeof(char));
        circular->head += nelements;
    }
    // Remove with wrap-around
    else {
        // Elements at the right side of the buffer
        int right = circular->capacity - circular->head;
        memset(circular->buffer + circular->head, ' ', right * sizeof(char));

        // Elements at the left side of the buffer
        int left = nelements - right;
        memset(circular->buffer, ' ', left * sizeof(char));

        circular->head += nelements;
        circular->head %= circular->capacity;   // can just use -= for efficiency
    }

    return true;
}

// Print a representation of a circular buffer
void print(circular_t* circular) {
    if(circular == NULL) {
        printf("Bad argument\n");
    }

    printf("Capacity:  %2d\n", circular->capacity);
    printf("Available: %2d\n", available(circular));
    printf("Head:      %2d\n", circular->head);
    printf("Tail:      %2d\n", circular->tail);

    printf("Contents:\n");
    for(int i = 0; i < circular->capacity; i++) {
        printf("%2d ", i);
    }
    printf("\n");
    for(int i = 0; i < circular->capacity; i++) {
        printf("%2c ", circular->buffer[i]);
    }
    printf("\n");
    for(int i = 0; i < circular->capacity; i++) {
        if(circular->head == i) {
            printf("%*s", i*3+2, "H");
        }
    }
    printf("\n");
    for(int i = 0; i < circular->capacity; i++) {
        if(circular->tail == i) {
            printf("%*s", i*3+2, "T");
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

    // Write some data to the tail
    printf("Write some data to the tail:\n");
    int i;
    for(i = 0; i < 10; i++) {
        char c = 'a' + i;
        write(circular, &c, 1);
    }
    print(circular);

    // Write some more data to the tail
    printf("Write some more data to the tail:\n");
    char* data = "klmnopq";
    write(circular, data, strlen(data));
    print(circular);

    // Try to write more data than there is space available
    printf("Try to write more data than there is space available:\n");
    data = "rstuvwxyz";
    write(circular, data, strlen(data));
    print(circular);

    // Try to read more data than exist in the buffer
    printf("Try to read more data than exist in the buffer:\n");
    read(circular, 19);
    print(circular);

    // Read some data from the head
    printf("Read some data from the head:\n");
    read(circular, 10);
    print(circular);

    // Write some more data to the tail, with wrap-around
    printf("Write some more data to the tail, with wrap-around:\n");
    data = "rstuvwxyz";
    write(circular, data, strlen(data));
    print(circular);

//    // Completely fill the buffer
//    printf("Completely fill the buffer:\n");
//    data = "xy";
//    write(circular, data, strlen(data));
//    print(circular);

    // Read some data from the head, with wrap-around
    printf("Read some data from the head, with wrap-around:\n");
    read(circular, 12);
    print(circular);

    // Clean-up
    destroy(&circular);
    return EXIT_SUCCESS;
}




//Ring buffer / Circular buffer
//1024 bytes.
//
//Linear memory view case 1:
//+----------------------------------------------+
//|NNNN          DDDDDDDDDDNNNNNNNNNN         end|
//+----------------------------------------------+
//           Tail^         ^ Head
//Linear memory view case 2:
//+----------------------------------------------+
//|DDDDDDDDDD                              DDDDDD|
//+----------------------------------------------+
//           ^Head                         ^Tail
//
//Some new data: pointnewData:
//+-----------+
//|DDDDDDDDDDD|
//+-----------+
//
//
//bool appendToBuf(const char *newData, size_t newDataSize)
//// is free space
////    head idx/ptr
////    tail idx/prt
////        size - (head - tail) but take into account wrap around
//                if head > tail    ==> case 1
//                    if buflen - (head - tail) > newDataSize
//                        have enough space
//                    else
//                        fail
//
//                if head < tail    ==> case 2
//                    left = head
//                    rigth = buflen - tail
//                    freespace = right - left
//                    if freespace > newDataSize
//
//                    if tail - head > newDataSize
//                    //if (buflen - tail) - head > newDataSize
//                        have enough space
//                    else
//                        fail
//
//
//                if head > tail    ==> case 1
////                    if buflen - head > newDataSize    ==> case 1
////                        mempcy(head, newData, newDataSize)
////                    else
////                        fit = buflen - head
//                          mempcy(head, newData, fit)
//                          mempcy(start, newData, newDataSize -fit)
//
//                if head < tail    ==> case 2
//                    if tail - head > newDataSize
//                        mempcy(head, newData, newDataSize)
//
////            if head > tail    ==> case 1
////                if tail > newDataSize
////                    simple memcpy
////                else
////                    part of data at left, part of data at right, resulting head < tail
////            if head < tail    ==> case 2
////                     simple memcpy
//
//
//// if it fits
////     memcpy(tail, newData, newDataSize) but take into account wrap around
////     update tail
////     return true
//
//// if it does not fit
////    return false
////    assume someone else frees newData

