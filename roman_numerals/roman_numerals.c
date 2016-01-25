// Convert Roman numerals to decimal values and vice versa
//
// See https://en.wikipedia.org/wiki/Roman_numerals
//
// Note that when converting a Roman numeral string to a decimal value it is
// easier to process the string from right-to-left rather than left-to-right:
//
//  right-to-left : only need the current and previous numeral to decide whether
//                  to add or subtract
//
//  left-to-right : need the current and next two numerals to decide whether to
//                  add or subtract

#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS, NULL, size_t
#include <string.h> // For strlen

#define NELEMENTS(a)    (sizeof(a) / sizeof(a[0]))

// Note that 1888 is 13 numerals: MDCCCLXXXVIII
#define MAX_NUMERALS    13

// Convert a single Roman numeral to decimal
unsigned int char_to_decimal(char numeral) {
    unsigned int result = 0;

    switch(numeral) {
    case 'I': result = 1;    break;
    case 'V': result = 5;    break;
    case 'X': result = 10;   break;
    case 'L': result = 50;   break;
    case 'C': result = 100;  break;
    case 'D': result = 500;  break;
    case 'M': result = 1000; break;
    default: printf("Invalid Roman numeral: %c\n", numeral);
    }

    return result;
}

// Convert a Roman numeral string to a decimal value
unsigned int string_to_decimal(char* string) {
    if(string == NULL) {
        printf("Bad argument\n");
        return 0;
    }

    unsigned int curr = 0;
    unsigned int prev = 0;
    unsigned int sum  = 0;
    for(size_t i = strlen(string); i > 0; i--) {
        // Value of the current numeral
        curr = char_to_decimal(string[i - 1]);
        if(curr >= prev) {
            // E.g. XI => curr=10, prev=1 => 10+1 => 11
            sum += curr;
        }
        else {
            // Vn, Ln and Dn are all invalid combinations
            if(curr == 5 || curr == 50 || curr == 500) {
                return 0;
            }

            // Ratio of prev to curr should be 10 or less
            if((curr * 10) < prev) {
                return 0;
            }

            // E.g. IX => curr=1, prev=10 => 10-1 => 9
            sum -= curr;
        }

        // Next iteration
        prev = curr;
    }
    return sum;
}

// Convert a decimal value to a Roman numeral string
char* decimal_to_string(unsigned int decimal) {
    if(decimal == 0) {
        printf("Bad argument\n");
        return NULL;
    }

    char* string = malloc(MAX_NUMERALS + 1);
    if(string == NULL) {
        printf("Bad malloc\n");
        return NULL;
    }

    // Map of decimal values to single Roman numerals
    typedef struct map_t {
        unsigned int decimal;
        char         numeral;
    } map_t;
    map_t map[] = {
        { 1000, 'M' },
        { 500,  'D' },
        { 100,  'C' },
        { 50,   'L' },
        { 10,   'X' },
        { 5,    'V' },
        { 1,    'I' },
    };

    // Work from M to I, subtracting numeral values whilst building the string
    size_t j = 0;
    for(size_t i = 0; i < NELEMENTS(map) && decimal > 0; i++) {
        // Use the current numeral as many times as possible
        while(map[i].decimal <= decimal) {
                string[j++] = map[i].numeral;
                decimal -= map[i].decimal;
        }

        // Try subtractive notation before moving to the next lower numeral...

        // Subtractive notation using non-adjacent numerals e.g. IX, XC, CM
        if(((i + 2) < NELEMENTS(map)) &&
           ((map[i].decimal != 50)  && (map[i].decimal != 500)) && // VL and LD are not valid
           (decimal >= map[i].decimal - map[i+2].decimal)) {
            string[j++] = map[i+2].numeral;
            string[j++] = map[i].numeral;
            decimal -= map[i].decimal - map[i+2].decimal;
        }
        // Subtractive notation using adjacent numerals e.g. IV, XL or CD
        else if(((i + 1) < NELEMENTS(map)) &&
                ((map[i].decimal / map[i+1].decimal) == 5) &&   // VX, LC and DM are not valid
                (decimal >= map[i].decimal - map[i+1].decimal)) {
            string[j++] = map[i+1].numeral;
            string[j++] = map[i].numeral;
            decimal -= map[i].decimal - map[i+1].decimal;
        }
    }
    string[j] = '\0';

    return string;
}

// Common test function
void test(char* string, unsigned int decimal) {
    // Convert in both directions
    unsigned int s2d = string_to_decimal(string);
    char*        d2s = decimal_to_string(decimal);
    if((s2d == decimal) && (strcmp(d2s, string) == 0)) {
        printf("%13s %u\n", d2s, s2d);
    }
    else {
        printf("%13s - bad conversion, should be %s\n", string, d2s);
    }
}

int main(void) {
    // Individual numerals
    printf("Individual numerals:\n");
    test("I", 1);
    test("V", 5);
    test("X", 10);
    test("L", 50);
    test("C", 100);
    test("D", 500);
    test("M", 1000);
    printf("\n");

    // Valid combinations for subtractive notation
    printf("Valid combinations for subtractive notation:\n");
    test("IV", 4);
    test("IX", 9);
    test("XL", 40);
    test("XC", 90);
    test("CD", 400);
    test("CM", 900);
    printf("\n");

    // Numbers 1 to 10
    printf("Numbers 1 to 10:\n");
    test("I",    1);
    test("II",   2);
    test("III",  3);
    test("IV",   4);
    test("V",    5);
    test("VI",   6);
    test("VII",  7);
    test("VIII", 8);
    test("IX",   9);
    test("X",    10);
    printf("\n");

    // Various years
    printf("Various years:\n");
    test("MDCCCLXXXVIII", 1888); // a very long year
    test("MCMIV",         1904); // from Wikipedia
    test("MCMLIV",        1954); // from Wikipedia - as in the trailer for the movie The Last Time I Saw Paris
    test("MCMXC",         1990); // from Wikipedia - used as the title of musical project Enigma's debut album MCMXC a.D., named after the year of its release.
    test("MMXIV",         2014); // from Wikipedia - the year of the games of the XXII (22nd) Olympic Winter Games (in Sochi)
    printf("\n");

    // Invalid combinations for subtractive notation
    printf("Invalid combinations for subtractive notation:\n");
    test("IL", 49);  // should be XLIX
    test("IC", 99);  // should be XCIX
    test("ID", 499); // should be CDXCIX
    test("IM", 999); // should be CMXCIX
    test("VX", 5);   // should be V
    test("VL", 45);  // should be XLV
    test("VC", 95);  // should be XCV
    test("VD", 495); // should be CDXCV
    test("VM", 995); // should be CMXCV
    test("XD", 490); // should be CDXC
    test("XM", 990); // should be CMXC
    test("LC", 50);  // should be L
    test("LD", 450); // should be CDL
    test("LM", 950); // should be CML
    test("DM", 500); // should be D
    printf("\n");

    // Valid counterparts to the invalid combinations
    printf("Valid counterparts to the invalid combinations:\n");
    test("XLIX",   49);
    test("XCIX",   99);
    test("CDXCIX", 499);
    test("CMXCIX", 999);
    test("V",      5);
    test("XLV",    45);
    test("XCV",    95);
    test("CDXCV",  495);
    test("CMXCV",  995);
    test("CDXC",   490);
    test("CMXC",   990);
    test("L",      50);
    test("CDL",    450);
    test("CML",    950);
    test("D",      500);
    printf("\n");

    return EXIT_SUCCESS;
}
