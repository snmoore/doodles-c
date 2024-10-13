#!/usr/bin/env python3

# Count the number of times each word appears in a text file.
#
# For example, given a file containing:
#    predriver dichlamydeous predriver magnetoplasmadynamic undistrustful dichlamydeous undistrustful predriver undistrustful predriver
#
# The word counts are:
#    predriver            4
#    dichlamydeous        2
#    magnetoplasmadynamic 1
#    undistrustful        3

import argparse
import os.path

def count_words(file_path: str) -> dict:
    """Count the number of times each word appears in a text file.
    
    Parameters:
        file_path (str) : file path for the file to be processed.
        
    Returns:
        A dictionary containing the words as the keys and the counts as the values.
    """

    # Dictionary containing the words as the keys and the counts as the values.
    counts = dict()
    
    # Open the file for reading.
    with open(file_path, 'r') as file:
        # Read each word, separating upon whitespace.
        words = file.read().split()
    
        # Count each word read from the file.
        for word in words:
            if word in counts:
                counts[word] += 1
            else:
                counts[word] = 1

    return counts


def pretty_print(counts: dict, file_path: str, add_header_and_footer: bool) -> None:
    """Print the word counts.
    
    Parameters:
        counts (dict)                : dictionary containing the words as the keys and the counts as the values.
        file_path (str)              : file path for the file from which the counts were determined.
        add_header_and_footer (bool) : true to add a header and footer e.g. to separate the output for multiple files;
                                       false to omit the header and footer.
    """
    
    # Add a header when processing multiple files.
    if add_header_and_footer:
        print(f"{os.path.basename(file_path)}")
        print("=" * len(os.path.basename(file_path)))    
    
    # Find the length of the longest word, to help align the output.
    max_length = 0
    for word in counts:
        if len(word) > max_length:
            max_length = len(word)     
    
    # Print the word counts.
    for word, count in counts.items():
        print(f"{word:{max_length}} {count}")

    # Add a footer when processing multiple files.
    if add_header_and_footer:
        print("")


def main():
    """Main function when run as a script.
    
    Parse the command line to determine the set of files to be processed, then process each file in turn.
    """
    
    # Parse the command line.
    parser = argparse.ArgumentParser(description="Count the number of times each word appears in a text file.")
    parser.add_argument('filename', nargs='+')
    args = parser.parse_args()
    
    # If there are multiple files then add a header and footer when printing.
    add_header_and_footer = len(args.filename) > 1
    
    # Process each file.
    for file_path in args.filename:
        # Count the number of times each word appears in the file.
        counts = count_words(file_path)
          
        # Print the word counts.
        pretty_print(counts, file_path, add_header_and_footer)     

if __name__ == '__main__':
    main()
