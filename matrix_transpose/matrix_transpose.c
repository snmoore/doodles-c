// Transpose a matrix with dimensions m x n to dimensions n x m

#include <errno.h>  // For errno
#include <string.h> // For strerror
#include <stdio.h>  // For printf
#include <stdlib.h> // For calloc, free, EXIT_SUCCESS

int * transpose(int *matrix, unsigned int rows, unsigned int cols) {
    // Resulting matrix has dimensions cols x rows
    int *transp = calloc(cols * rows, sizeof(int));
    if(transp == NULL)
    {
        printf("malloc failed: %s", strerror(errno));
        return NULL;
    }

    // Transpose the matrix
    for(unsigned int r = 0; r < rows; r++) {
        for(unsigned int c = 0; c < cols; c++) {
            transp[c*rows + r] =
            matrix[r*cols + c];
        }
    }

    return transp;
}

// Utility function to print a matrix
void print(char *label, int *matrix, unsigned int rows, unsigned int cols) {
    printf("\n%s\n", label);
    for(unsigned int r = 0; r < rows; r++) {
        for(unsigned int c = 0; c < cols; c++) {
            printf("%3d ", matrix[r*cols + c]);
        }
        printf("\n");
    }
}

int main(void) {
    int a[2][4] = {
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 }
    };

    int b[4][2] = {
        { 1, 2 },
        { 3, 4 },
        { 5, 6 },
        { 7, 8 }
    };

    int c[3][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 }
    };

    int * m = NULL;

    // Transpose a to a'
    print("Matrix a:",  (int*)a, 2, 4);
    m = transpose((int*)a, 2, 4);
    if(m != NULL) {
        print("Matrix a':", (int*)m, 4, 2);
        free(m);
    }

    // Transpose b to b'
    print("Matrix b:",  (int*)b, 4, 2);
    m = transpose((int*)b, 4, 2);
    if(m != NULL) {
        print("Matrix b':", (int*)m, 2, 4);
        free(m);
    }

    // Transpose c to c'
    print("Matrix c:",  (int*)c, 3, 3);
    m = transpose((int*)c, 3, 3);
    if(m != NULL) {
        print("Matrix c':", (int*)m, 3, 3);
        free(m);
    }

    return EXIT_SUCCESS;
}
