// Verify the initialization of static and non-static variables
//
// This resulted from a job interview where the interviewer insisted that
//      static unsigned char a[256];
// was not initialized to all 0, but rather contained random data.
//
//
// ISO/IEC 9899:1999 section 6.2.4 'Storage durations of objects' says:
//
//      An object whose identifier is declared with external or internal
//      linkage, or with the storage-class specifier static has static storage
//      duration. Its lifetime is the entire execution of the program and its
//      stored value is initialized only once, prior to program startup.
//
// Hence extern or static globals and static locals all have static storage
// duration, and all are initialized once.
//
//
// ISO/IEC 9899:1999 section 6.7.8 'Initialization' says:
//
//      If an object that has static storage duration is not initialized
//      explicitly, then:
//      - if it has pointer type, it is initialized to a null pointer;
//      - if it has arithmetic type, it is initialized to (positive or unsigned)
//        zero;
//      - if it is an aggregate, every member is initialized (recursively)
//        according to these rules;
//      - if it is a union, the first named member is initialized (recursively)
//        according to these rules.
//
// Hence static unsigned char a[256]; is initialized to zero.
//
//
// ISO/IEC 9899:1999 section 6.7.8 'Initialization' also says:
//      If there are fewer initializers in a brace-enclosed list than there are
//      elements or members of an aggregate, or fewer characters in a string
//      literal used to initialize an array of known size than there are
//      elements in the array, the remainder of the aggregate shall be
//      initialized implicitly the same as objects that have static storage
//      duration.
//
// Hence static unsigned char a[256]={1}; is initialized to 1 followed by all 0

#include <stdint.h> // For uint8_t
#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS, NULL, size_t

#define NELEMENTS(a) (sizeof(a)/sizeof(a[0]))

uint8_t g[10];              // initialized to all 0, storage class is static by default
uint8_t h[10] = {0};        // initialized to all 0, storage class is static by default
uint8_t i[10] = {1};        // initialized to 1 followed by all 0, storage class is static by default

static uint8_t j[10];       // initialized to all 0
static uint8_t k[10] = {0}; // initialized to all 0
static uint8_t l[10] = {1}; // initialized to 1 followed by all 0

// Print the contents of an array
void print(const char* msg, uint8_t* data, size_t nelements) {
    if(data == NULL) {
        return;
    }

    printf("%s", msg);
    for(size_t i = 0; i < nelements; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t a[10];              // uninitialized, likely random data
    uint8_t b[10] = {0};        // initialized to all 0
    uint8_t c[10] = {1};        // initialized to 1 followed by all 0

    static uint8_t d[10];       // initialized to all 0
    static uint8_t e[10] = {0}; // initialized to all 0
    static uint8_t f[10] = {1}; // initialized to 1 followed by all 0

    print("       uint8_t a[10];                   ", a, NELEMENTS(a));
    print("       uint8_t b[10] = {0};             ", b, NELEMENTS(b));
    print("       uint8_t c[10] = {1};             ", c, NELEMENTS(c));
    printf("\n");

    print("static uint8_t d[10];                   ", d, NELEMENTS(d));
    print("static uint8_t e[10] = {0};             ", e, NELEMENTS(e));
    print("static uint8_t f[10] = {1};             ", f, NELEMENTS(f));
    printf("\n");

    print("       uint8_t g[10];       (module)    ", g, NELEMENTS(g));
    print("       uint8_t h[10] = {0}; (module)    ", h, NELEMENTS(h));
    print("       uint8_t i[10] = {1}; (module)    ", i, NELEMENTS(i));
    printf("\n");

    print("static uint8_t j[10];       (module)    ", j, NELEMENTS(j));
    print("static uint8_t k[10] = {0}; (module)    ", k, NELEMENTS(k));
    print("static uint8_t l[10] = {1}; (module)    ", l, NELEMENTS(l));
    printf("\n");

    return EXIT_SUCCESS;
}
