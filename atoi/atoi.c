// Convert a string to an integer
//
// See http://pubs.opengroup.org/onlinepubs/009695399/functions/atoi.html

#include <ctype.h>  // For isdigit
#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS, NULL, atoi

#define NELEMENTS(a)    (sizeof(a) / sizeof(a[0]))

// Convert a string to an integer
int my_atoi(const char *str) {
    int sign  = 1;
    int value = 0;

    if(str != NULL) {
        // Process the sign, if present
        if(*str == '-') {
            sign = -1;
            str++;
        }
        else if(*str == '+') {
            str++;
        }

        // Convert the digits
        while(isdigit(*str)) {
            value *= 10;
            value += *str - '0';
            str++;
        }
    }

    return value * sign;
}

int main(void) {
    char *strings[] = { "0", "11", "+11", "-11", "2147483647", "abc", "" };
                                                 /* INT_MAX */

    printf("String\tMy atio\tatoi\n");
    for(size_t i = 0; i < NELEMENTS(strings); i++) {
        printf("\"%s\" %d %d\n", strings[i], my_atoi(strings[i]), atoi(strings[i]));
    }

    // Note that atoi fails with NULL e.g. causes a segmentation fault
    printf("NULL %d SEGV\n", my_atoi(NULL));

    return EXIT_SUCCESS;
}
