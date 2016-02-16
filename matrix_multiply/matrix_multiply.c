// Multiply two matrices with dimensions m x n and n x p
//
// The resulting matrix should have dimensions m x p

#include <errno.h>  // For errno
#include <string.h> // For strerror
#include <stdio.h>  // For printf
#include <stdlib.h> // For calloc, free, EXIT_SUCCESS

int * multiply(int *a, unsigned int arows, unsigned int acols,
               int *b, unsigned int brows, unsigned int bcols) {
    // To multiply matrices the dimensions need to be m x n and n x p
    if(acols != brows)
    {
        printf("Cannot multiply matrices with dimensions %d x %d and %d x %d\n", arows, acols, brows, bcols);
        return NULL;
    }

    // Resulting matrix has dimensions m x p
    int *c = calloc(arows * bcols, sizeof(int));
    if(c == NULL)
    {
        printf("malloc failed: %s", strerror(errno));
        return NULL;
    }

    // Multiply the matrices
    for(unsigned int m = 0; m < arows; m++) {
        for(unsigned int p = 0; p < bcols; p++) {
            for(unsigned int n = 0; n < acols; n++) {
                c[m*bcols + p] += a[m*acols + n] * b[n*bcols + p];
            }
        }
    }
    return c;
}

// Utility function to print a matrix
void print(char *label, int *matrix, unsigned int rows, unsigned int cols) {
    printf("\n%s\n", label);
    for(unsigned int m = 0; m < rows; m++) {
        for(unsigned int n = 0; n < cols; n++) {
            printf("%3d ", matrix[m*cols + n]);
        }
        printf("\n");
    }
}

int main(void) {
    int a[2][4] = {
        { -1,  2, -4, 8 },
        { -3, -5,  7, 9 }
    };

    int b[4][2] = {
        {  0,  1 },
        { -2,  3 },
        {  4, -5 },
        { -6, -7 }
    };

    int c[3][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 }
    };

    int * m = NULL;

    // a * b
    printf("\na * b:\n======\n");
    print("Matrix a:", (int*)a, 2, 4);
    print("Matrix b:", (int*)b, 4, 2);
    m = multiply((int*)a, 2, 4, (int*)b, 4, 2);
    if(m != NULL) {
        print("Result:",   (int*)m, 2, 2);
        free(m);
    }

    // a * c is expected to fail
    printf("\na * c:\n======\n");
    print("Matrix a:", (int*)a, 2, 4);
    print("Matrix c:", (int*)c, 3, 3);
    m = multiply((int*)a, 2, 4, (int*)c, 3, 3);
    if(m != NULL) {
        print("Result:",   (int*)m, 2, 3);
        free(m);
    }

    // b * a
    printf("\nb * a:\n======\n");
    print("Matrix b:", (int*)b, 4, 2);
    print("Matrix a:", (int*)a, 2, 4);
    m = multiply((int*)b, 4, 2, (int*)a, 2, 4);
    if(m != NULL) {
        print("Result:",   (int*)m, 4, 4);
        free(m);
    }

    // b * c is expected to fail
    printf("\nb * c:\n======\n");
    print("Matrix b:", (int*)b, 4, 2);
    print("Matrix c:", (int*)c, 3, 3);
    m = multiply((int*)b, 4, 2, (int*)c, 3, 3);
    if(m != NULL) {
        print("Result:",   (int*)m, 4, 3);
        free(m);
    }

    return EXIT_SUCCESS;
}
