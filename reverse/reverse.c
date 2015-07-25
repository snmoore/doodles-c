// Reverse words and sentences
//
// These simple implementations have many problems with:
//  Strings that are not NUL terminated
//  Variable length encodings such as UTF8 or UTF16 etc
//  Ideographic languages such as Chinese or Korean etc
//  Languages with conjoining characters such as Arabic etc
//  Word separators other than space such as tab, nbsp, punctuation characters

#include <ctype.h>      // For isspace
#include <stdio.h>      // For printf
#include <stdlib.h>     // For EXIT_SUCCESS
#include <string.h>     // For strlen

// Reverse a string in place using a temporary variable
static char *reverse_temp(char *string, size_t length)
{
    if ((string != NULL) && (string[0] != '\0') && (length > 0))
    {
        char temp;
        for (size_t i = 0, k = length - 1; i < length / 2; i++, k--)
        {
            temp      = string[k];
            string[k] = string[i];
            string[i] = temp;
        }
    }

    return string;
}

// Reverse a string in place using a temporary variable and pointers
static char *reverse_temp_pointer(char *string, size_t length)
{
    if ((string != NULL) && (string[0] != '\0') && (length > 0))
    {
        char temp;
        char *start  = string;
        char *right = &string[length - 1];
        while (right - start > 0)
        {
            temp     = *right;
            *right-- = *start;
            *start++  = temp;
        }
    }

    return string;
}

// Reverse a string in place using XOR
static char *reverse_xor(char *string, size_t length)
{
    if ((string != NULL) && (string[0] != '\0') && (length > 0))
    {
        for (size_t i = 0, k = length - 1; i < length / 2; i++, k--)
        {
            string[k] ^= string[i];
            string[i] ^= string[k];
            string[k] ^= string[i];
        }
    }

    return string;
}

// Reverse the order of words in a sentence
static char *reverse_sentence(char *sentence, size_t length)
{
    if ((sentence != NULL) && (sentence[0] != '\0') && (length > 0))
    {
        // Find each word in the sentence and reverse it
        size_t i;
        char *start = NULL;
        for(i = 0; i < length; i++)
        {
            if(isspace(sentence[i]))
            {
                if(start != NULL)
                {
                    // Found the end of a word, reverse it
                    reverse_xor(start, sentence + i - start);
                    start = NULL;
                }
            }
            else
            {
                if(start == NULL)
                {
                    // Found the start of a word
                    start = sentence + i;
                }
            }
        }
        // Reverse the final word
        reverse_xor(start, sentence + i - start);

        // Reverse all of the characters in the sentence
        sentence = reverse_xor(sentence, length);
    }

    return sentence;
}

int main(void)
{
    char word[]     = "Hello";
    char sentence[] = "Reverse the order of words in a sentence";

    printf("Reverse with a temporary variable:\n%s -> ", word);
    printf("%s\n\n", reverse_temp(word, strlen(word)));

    printf("Reverse with a temporary variable and pointers:\n%s -> ", word);
    printf("%s\n\n", reverse_temp_pointer(word, strlen(word)));

    printf("Reverse with XOR:\n%s -> ", word);
    printf("%s\n\n", reverse_xor(word, strlen(word)));

    printf("Reverse a sentence:\n%s -> ", sentence);
    printf("%s\n\n", reverse_sentence(sentence, strlen(sentence)));

    return EXIT_SUCCESS;
}

