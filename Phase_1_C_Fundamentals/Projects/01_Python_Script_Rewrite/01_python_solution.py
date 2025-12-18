#!/usr/bin/env python3
"""
Word Counter and Text Statistics
A simple Python script that reads a file and displays text statistics.
"""

import sys


def count_text_statistics(filename):
    """
    Read a file and calculate text statistics.
    Returns a dictionary with counts.
    """

    try:
        with open(filename, 'r') as file:
            total_chars = 0
            total_words = 0
            total_lines = 0
            longest_word = ""
            longest_length = 0

            # Read file line by line
            for line in file:
                total_lines += 1

                # Count characters (including newline)
                total_chars += len(line)

                # Count words in this line
                words = line.split()
                total_words += len(words)

                # Check each word for longest
                for word in words:
                    # Remove punctuation for fair comparison
                    clean_word = ''.join(c for c in word if c.isalnum())
                    word_length = len(clean_word)

                    if word_length > longest_length:
                        longest_length = word_length
                        longest_word = clean_word

            return {
                'filename': filename,
                'total_chars': total_chars,
                'total_words': total_words,
                'total_lines': total_lines,
                'longest_word': longest_word,
                'longest_length': longest_length
            }

    except FileNotFoundError:
        print(f"Error: File '{filename}' not found")
        sys.exit(1)
    except Exception as e:
        print(f"Error reading file: {e}")
        sys.exit(1)


def display_statistics(stats):
    """Display the statistics in a formatted way."""

    print("=== Text Statistics ===")
    print(f"File: {stats['filename']}")
    print(f"Total characters: {stats['total_chars']}")
    print(f"Total words: {stats['total_words']}")
    print(f"Total lines: {stats['total_lines']}")

    if stats['longest_word']:
        print(f"Longest word: {stats['longest_word']} ({stats['longest_length']} characters)")
    else:
        print("Longest word: (none)")


def main():
    """Main entry point."""

    # # Check command line arguments
    # if len(sys.argv) != 2:
    #     print("Usage: python3 word_counter.py <filename>")
    #     print("Example: python3 word_counter.py sample_input.txt")
    #     sys.exit(1)

    filename = "sample_input.txt"

    # Count statistics
    stats = count_text_statistics(filename)

    # Display results
    display_statistics(stats)


if __name__ == "__main__":
    main()
