// Count the number of times a word appears in a file

#include <ctype.h>      /* For isspace */
#include <errno.h>      /* For errno */
#include <fcntl.h>      /* For open */
#include <stddef.h>     /* For size_t */
#include <stdio.h>      /* For printf */
#include <stdlib.h>     /* For EXIT_FAILURE, EXIT_SUCCESS, malloc */
#include <string.h>     /* For strerror */
#include <unistd.h>     /* For close */
#include <sys/stat.h>   /* For stat */
#include "fnv64.h"      /* For fnv64 */

int main(int argc, char *argv[]) {
    // Process the command line
    if(argc != 2) {
        printf("Usage: ./word_count FILE\n");
        return EXIT_FAILURE;
    }

    // Get the size of the file
    struct stat info;
    const int status = stat(argv[1], &info);
    if(status == -1) {
        printf("Failed to get the size of the file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Allocate a buffer into which the entire contents of the file can be read
    char * buffer = malloc(info.st_size);
    if(buffer == NULL) {
        printf("Failed to allocate memory: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Open the file for reading
    const int file = open(argv[1], O_RDONLY);
    if(file == -1) {
        printf("Failed to open file: %s\n", strerror(errno));
        free(buffer);
        return EXIT_FAILURE;
    }

    // Read the entire file contents into memory
    const ssize_t bytes_read = read(file, buffer, info.st_size);
    if(bytes_read == -1) {
        printf("Failed to read file: %s\n", strerror(errno));
        free(buffer);
        close(file);
        return EXIT_FAILURE;
    }
    if(bytes_read != info.st_size) {
        printf("Bad read from file: read %zu of %llu bytes\n", bytes_read, info.st_size);
    }

    // Look for each word
    char word[100] = "";
    size_t length = 0;
    size_t max_length = 0;
    for(size_t offset = 0; offset < (size_t)bytes_read; offset++) {
        // Look for a word separator
        if(isspace(buffer[offset]) != 0) {
            // End the current word
            word[length] = '\0';

            // Keep track of the maximum word length
            // TODO: Do this later when dumping from the hash table
            if(length > max_length) {
                max_length = length;
            }

            // Compute the 64-bit FNV-1a hash of the word
            const uint64_t hash = fnv64((uint8_t*)word, length);

            // TODO: Track the word count in a hash table
            printf("0x%016llx %s\n", hash, word);

            // Next word
            length = 0;
        }
        else {
            // Continue the current word
            word[length++] = buffer[offset];
        }
    }
    printf("Max length: %zu\n", max_length);

    // Clean up
    free(buffer);
    close(file);

    return EXIT_SUCCESS;
}
