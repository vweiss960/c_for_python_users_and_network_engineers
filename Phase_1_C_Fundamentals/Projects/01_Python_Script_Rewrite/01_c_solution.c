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
 * Extract a single word from the line, removing punctuation.
 * Returns the length of the word, or 0 if at end of line.
 */
int extract_word(const char *line, int start_pos, char *word_buffer) {
    int word_pos = 0;

    // Skip whitespace
    while (start_pos < (int)strlen(line) && isspace(line[start_pos])) {
        start_pos++;
    }

    // Copy alphanumeric characters only
    while (start_pos < (int)strlen(line) && word_pos < MAX_WORD_LENGTH - 1) {
        char c = line[start_pos];

        if (isalnum(c)) {
            word_buffer[word_pos++] = c;
        } else if (isspace(c)) {
            break;  // End of word
        }

        start_pos++;
    }

    word_buffer[word_pos] = '\0';  // Null terminate
    return word_pos;                // Return word length
}

/**
 * Count words in a line and extract them one by one.
 * Update longest_word if any word is longer than current longest.
 */
int process_line(const char *line, char *longest_word, int *longest_length) {
    int word_count = 0;
    int pos = 0;
    char word[MAX_WORD_LENGTH];

    // Extract words from the line
    while (pos < (int)strlen(line)) {
        int word_len = extract_word(line, pos, word);

        if (word_len == 0) {
            break;  // No more words
        }

        word_count++;

        // Update longest word if this one is longer
        if (word_len > *longest_length) {
            *longest_length = word_len;
            strncpy(longest_word, word, MAX_WORD_LENGTH - 1);
            longest_word[MAX_WORD_LENGTH - 1] = '\0';  // Ensure null termination
        }

        // Move position past this word and any whitespace
        pos += strlen(word);
    }

    return word_count;
}

/**
 * Read a file and calculate text statistics.
 */
TextStatistics count_text_statistics(const char *filename) {
    TextStatistics stats;

    // Initialize structure
    strncpy(stats.filename, filename, sizeof(stats.filename) - 1);
    stats.filename[sizeof(stats.filename) - 1] = '\0';
    stats.total_chars = 0;
    stats.total_words = 0;
    stats.total_lines = 0;
    stats.longest_word[0] = '\0';
    stats.longest_length = 0;

    // Open file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        perror("fopen");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];

    // Read file line by line
    while (fgets(line, sizeof(line), file) != NULL) {
        stats.total_lines++;

        // Count characters including newline
        stats.total_chars += strlen(line);

        // Process this line for words
        int words_in_line = process_line(line, stats.longest_word, &stats.longest_length);
        stats.total_words += words_in_line;
    }

    // Check for read errors
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        exit(1);
    }

    // Clean up
    fclose(file);

    return stats;
}

/**
 * Display statistics in formatted output.
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
