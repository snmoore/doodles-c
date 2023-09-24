// Count the number of times a word appears in a file.
//
// Work in progress:
//  This is currently implemented using a hash table with direct addressing, using an FNV1-a 16-bit hash function.
//  This is susceptible to hash collisions for different words e.g. "helled" and "tweesht" result in the same key.
//
// TODO:
//  Upgrade to a hash table with linear probing, or a chained hash table, together with a 32 or 64-bit hash function.

#include <ctype.h>      /* For isspace */
#include <errno.h>      /* For errno */
#include <fcntl.h>      /* For open */
#include <stddef.h>     /* For size_t */
#include <stdio.h>      /* For printf */
#include <stdint.h>     /* For uint16_t */
#include <stdlib.h>     /* For EXIT_FAILURE, EXIT_SUCCESS, malloc */
#include <string.h>     /* For strerror */
#include <unistd.h>     /* For close */
#include <sys/stat.h>   /* For stat */
#include "fnv16.h"      /* For fnv16 */
#include "hash_table.h" /* For hash_table */

// Type used to track a word in a hash table.
typedef struct word_tab {
    const char * string;
    uint16_t     count;
} word_t;

// Print a word from the hash table.
static size_t max_word_length = 0;
static void print_word(uint16_t key, void * const value);

// Entry point for the program.
int main(int argc, char *argv[]) {
    // Process the command line.
    if(argc != 2) {
        printf("Usage: ./word_count FILE\n");
        return EXIT_FAILURE;
    }

    // Get the size of the file.
    struct stat info;
    const int status = stat(argv[1], &info);
    if(status == -1) {
        printf("Failed to get the size of the file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Allocate a buffer into which the entire contents of the file can be read.
    char * buffer = malloc(info.st_size);
    if(buffer == NULL) {
        printf("Failed to allocate memory: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Open the file for reading.
    const int file = open(argv[1], O_RDONLY);
    if(file == -1) {
        printf("Failed to open file: %s\n", strerror(errno));
        free(buffer);
        return EXIT_FAILURE;
    }

    // Read the entire file contents into memory.
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

    //  Create a hash table i.e. allocate and initialise all memory.
    hash_table_t * table = hash_table_create(HASH_KEY_BITS_16, sizeof(word_t));
    if(table == NULL) {
        printf("Failed to create hash table\n");
    }

    // Look for each word.
    const char * start = buffer;
    for(size_t offset = 0; offset < (size_t)bytes_read; offset++) {
        // Look for a word separator.
        if(isspace(buffer[offset]) != 0) {
            // End the word.
            buffer[offset] = '\0';
            const size_t length = (buffer + offset) - start;

            // Track the word in the hash table.
            const word_t   word = { start, 1 };
            const uint16_t key  = fnv16((uint8_t*)start, length);
            bool inserted = hash_table_insert(table, key, sizeof(word), &word, false);
            if(!inserted) {
                // Insertion failed, which means the key already exists in the hash table.
                word_t existing_word;
                (void)hash_table_retrieve(table, key, sizeof(existing_word), &existing_word);

                // Check for a key collision.
                if(strcmp(existing_word.string, word.string) != 0) {
                    printf("Key collision: existing: %s new: %s\n", existing_word.string, word.string);
                    return EXIT_FAILURE;
                }

                // Update the count and re-insert into the hash table.
                existing_word.count++;
                inserted = hash_table_insert(table, key, sizeof(existing_word), &existing_word, true);
                if(!inserted) {
                    printf("Failed to insert word: %s\n", start);
                }
            }

            // Track the maximum word length; will be used later when printing the results.
            if(length > max_word_length) {
                max_word_length = length;
            }

            // Move on to the next word.
            start = &buffer[offset + 1];
        }
    }

    // Print the results.
    word_t word;
    hash_table_iterate(table, sizeof(word), &word, print_word);

    // Clean up.
    hash_table_destroy(&table);
    free(buffer);
    close(file);

    return EXIT_SUCCESS;
}

// Print a word from the hash table.
static void print_word(uint16_t key, void * const value) {
    (void)key;
    const word_t * const word = value;
    printf("%-*s %hu\n", (int)max_word_length, word->string, word->count);
}
