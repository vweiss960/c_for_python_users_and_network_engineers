#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_WORD_LENGTH 256

typedef struct {
    char filename[256];
    int total_chars;
    int total_words;
    int total_lines;
    char longest_word[MAX_WORD_LENGTH];
    int longest_length;
} TextStatistics;

/**
 * TODO: Write this function to extract a word from a line.
 *
 * The word should only contain alphanumeric characters (letters and digits).
 * Punctuation should be stripped.
 *
 * Parameters:
 *   line - the input line (do not modify)
 *   start_pos - position to start extracting from
 *   word_buffer - buffer to store the extracted word (you modify this)
 *
 * Returns:
 *   The length of the extracted word (0 if no word found)
 *
 * Hints:
 *   - Use isspace() to detect whitespace
 *   - Use isalnum() to detect alphanumeric characters
 *   - Remember to null-terminate the word buffer
 */
int extract_word(const char *line, int start_pos, char *word_buffer) {
    // YOUR CODE HERE
    return 0;
}

/**
 * TODO: Write this function to process a line of text.
 *
 * It should:
 * 1. Count how many words are in the line
 * 2. Update longest_word and longest_length if any word is longer than current longest
 *
 * Parameters:
 *   line - the input line
 *   longest_word - pointer to longest_word from the stats struct
 *   longest_length - pointer to longest_length from the stats struct
 *
 * Returns:
 *   The number of words found in the line
 *
 * Hints:
 *   - Call extract_word() to get each word
 *   - Use a while loop to extract words until there are no more
 *   - Use strncpy() to copy the longest word safely
 */
int process_line(const char *line, char *longest_word, int *longest_length) {
    // YOUR CODE HERE
    return 0;
}

/**
 * TODO: Write this function to read a file and gather statistics.
 *
 * It should:
 * 1. Open the file for reading
 * 2. Read each line and count:
 *    - Total characters
 *    - Total words (using process_line)
 *    - Total lines
 *    - Longest word
 * 3. Close the file and return statistics
 *
 * Hints:
 *   - Use fopen(filename, "r") to open
 *   - Use fgets(line, MAX_LINE_LENGTH, file) to read lines
 *   - Remember to check if fopen returned NULL
 *   - Remember to fclose() the file
 *   - strlen() gives the length of a string
 */
TextStatistics count_text_statistics(const char *filename) {
    TextStatistics stats;

    // Initialize the structure
    strncpy(stats.filename, filename, sizeof(stats.filename) - 1);
    stats.filename[sizeof(stats.filename) - 1] = '\0';
    stats.total_chars = 0;
    stats.total_words = 0;
    stats.total_lines = 0;
    stats.longest_word[0] = '\0';
    stats.longest_length = 0;

    // YOUR CODE HERE

    return stats;
}

/**
 * Display the statistics nicely.
 * This function is provided - you don't need to modify it.
 */
void display_statistics(const TextStatistics *stats) {
    printf("=== Text Statistics ===\n");
    printf("File: %s\n", stats->filename);
    printf("Total characters: %d\n", stats->total_chars);
    printf("Total words: %d\n", stats->total_words);
    printf("Total lines: %d\n", stats->total_lines);

    if (stats->longest_length > 0) {
        printf("Longest word: %s (%d characters)\n",
               stats->longest_word, stats->longest_length);
    } else {
        printf("Longest word: (none)\n");
    }
}

/**
 * Main entry point.
 * This function is provided - you don't need to modify it.
 */
int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        printf("Example: %s sample_input.txt\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    // Count statistics
    TextStatistics stats = count_text_statistics(filename);

    // Display results
    display_statistics(&stats);

    return 0;
}
