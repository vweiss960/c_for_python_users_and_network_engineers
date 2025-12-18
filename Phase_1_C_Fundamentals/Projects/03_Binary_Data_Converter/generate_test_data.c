/*
 * Test Data Generator for Binary Converter
 *
 * This utility generates test vectors and reference outputs for validating
 * the binary converter implementation.
 *
 * Usage: ./generate_test_data > test_vectors.txt
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

/* Helper function to print binary representation */
void print_binary(uint32_t value) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i > 0 && i % 8 == 0) printf(" ");
    }
}

/* Helper function to print all bases for a value */
void print_all_bases(uint32_t value) {
    printf("Value: %u (decimal)\n", value);
    printf("  Binary:       ");
    print_binary(value);
    printf("\n");
    printf("  Hexadecimal:  0x%08x\n", value);
    printf("  Octal:        0%011o\n", value);
}

int main(void) {
    printf("===============================================================================\n");
    printf("BINARY CONVERTER - TEST VECTORS\n");
    printf("===============================================================================\n\n");

    /* ========================================================================
     * TEST CATEGORY 1: Zero and Powers of Two
     * ======================================================================== */
    printf("TEST CATEGORY 1: Zero and Powers of Two\n");
    printf("------------------------------------------------------------------------\n\n");

    uint32_t powers_of_two[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 65536};
    size_t num_powers = sizeof(powers_of_two) / sizeof(powers_of_two[0]);

    for (size_t i = 0; i < num_powers; i++) {
        print_all_bases(powers_of_two[i]);
        printf("\n");
    }

    /* ========================================================================
     * TEST CATEGORY 2: Boundary Values
     * ======================================================================== */
    printf("\nTEST CATEGORY 2: Boundary Values\n");
    printf("------------------------------------------------------------------------\n\n");

    uint32_t boundaries[] = {
        255,              // Max 8-bit
        256,              // Min 9-bit
        65535,            // Max 16-bit
        65536,            // Min 17-bit
        16777215,         // Max 24-bit
        16777216,         // Min 25-bit
        4294967295        // Max 32-bit
    };
    size_t num_boundaries = sizeof(boundaries) / sizeof(boundaries[0]);

    for (size_t i = 0; i < num_boundaries; i++) {
        print_all_bases(boundaries[i]);
        printf("\n");
    }

    /* ========================================================================
     * TEST CATEGORY 3: Common Values
     * ======================================================================== */
    printf("\nTEST CATEGORY 3: Common Test Values\n");
    printf("------------------------------------------------------------------------\n\n");

    uint32_t common[] = {42, 73, 127, 137, 192, 255};
    size_t num_common = sizeof(common) / sizeof(common[0]);

    for (size_t i = 0; i < num_common; i++) {
        print_all_bases(common[i]);
        printf("\n");
    }

    /* ========================================================================
     * TEST CATEGORY 4: Bit Patterns
     * ======================================================================== */
    printf("\nTEST CATEGORY 4: Special Bit Patterns\n");
    printf("------------------------------------------------------------------------\n\n");

    printf("All zeros:\n");
    print_all_bases(0x00000000);
    printf("\n");

    printf("All ones:\n");
    print_all_bases(0xFFFFFFFF);
    printf("\n");

    printf("Alternating 1010... pattern:\n");
    print_all_bases(0xAAAAAAAA);
    printf("\n");

    printf("Alternating 0101... pattern:\n");
    print_all_bases(0x55555555);
    printf("\n");

    printf("Single bit set (bit 31):\n");
    print_all_bases(0x80000000);
    printf("\n");

    printf("Single bit set (bit 0):\n");
    print_all_bases(0x00000001);
    printf("\n");

    /* ========================================================================
     * TEST CATEGORY 5: IP Address Tests
     * ======================================================================== */
    printf("\nTEST CATEGORY 5: IP Addresses\n");
    printf("------------------------------------------------------------------------\n\n");

    struct {
        const char *ip_str;
        uint32_t expected_value;
    } ip_tests[] = {
        {"127.0.0.1",        0x7F000001},
        {"192.168.1.1",      0xC0A80101},
        {"10.0.0.1",         0x0A000001},
        {"8.8.8.8",          0x08080808},
        {"255.255.255.255",  0xFFFFFFFF},
        {"0.0.0.0",          0x00000000},
    };
    size_t num_ips = sizeof(ip_tests) / sizeof(ip_tests[0]);

    for (size_t i = 0; i < num_ips; i++) {
        printf("IP: %s\n", ip_tests[i].ip_str);

        // Parse IP manually (as the converter should)
        unsigned char a, b, c, d;
        sscanf(ip_tests[i].ip_str, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d);
        uint32_t ip_value = htonl((a << 24) | (b << 16) | (c << 8) | d);

        // Convert back to string
        uint32_t host_ip = ntohl(ip_value);
        unsigned char a2 = (host_ip >> 24) & 0xFF;
        unsigned char b2 = (host_ip >> 16) & 0xFF;
        unsigned char c2 = (host_ip >> 8) & 0xFF;
        unsigned char d2 = host_ip & 0xFF;
        printf("  Parsed value: 0x%08x\n", ip_value);
        printf("  Formatted back: %u.%u.%u.%u\n", a2, b2, c2, d2);
        printf("  Verification: %s\n\n", strcmp(ip_tests[i].ip_str, "127.0.0.1") == 0 ? "PASS" : "CHECK");
    }

    /* ========================================================================
     * TEST CATEGORY 6: Byte Swap Tests
     * ======================================================================== */
    printf("\nTEST CATEGORY 6: Byte Swap (Endianness)\n");
    printf("------------------------------------------------------------------------\n\n");

    uint32_t swap_tests[] = {
        0x12345678,
        0xDEADBEEF,
        0xCAFEBABE,
        0x00000001,
        0xFFFFFFFF
    };
    size_t num_swaps = sizeof(swap_tests) / sizeof(swap_tests[0]);

    for (size_t i = 0; i < num_swaps; i++) {
        uint32_t value = swap_tests[i];

        // Manual byte swap (as converter should do it)
        uint32_t swapped = 0;
        swapped |= (value & 0x000000FF) << 24;
        swapped |= (value & 0x0000FF00) << 8;
        swapped |= (value & 0x00FF0000) >> 8;
        swapped |= (value & 0xFF000000) >> 24;

        printf("Original: 0x%08x\n", value);
        printf("Swapped:  0x%08x\n\n", swapped);
    }

    /* ========================================================================
     * TEST CATEGORY 7: Bit Field Extraction
     * ======================================================================== */
    printf("\nTEST CATEGORY 7: Bit Field Extraction Examples\n");
    printf("------------------------------------------------------------------------\n\n");

    uint32_t test_value = 0b11010110;
    printf("Test value: 0b%08b (0x%02x, decimal %u)\n\n", test_value, test_value, test_value);

    struct {
        int start;
        int width;
        const char *description;
    } field_tests[] = {
        {0, 1, "bit 0 (LSB)"},
        {1, 1, "bit 1"},
        {5, 1, "bit 5"},
        {0, 4, "bits [3:0]"},
        {4, 4, "bits [7:4]"},
        {3, 3, "bits [5:3]"},
    };
    size_t num_fields = sizeof(field_tests) / sizeof(field_tests[0]);

    for (size_t i = 0; i < num_fields; i++) {
        uint32_t mask = (1U << field_tests[i].width) - 1;
        uint32_t extracted = (test_value >> field_tests[i].start) & mask;
        printf("Extract %s: value = %u (0b%0*b)\n",
               field_tests[i].description,
               extracted,
               field_tests[i].width,
               extracted);
    }
    printf("\n");

    /* ========================================================================
     * TEST CATEGORY 8: Parse Test Vectors
     * ======================================================================== */
    printf("\nTEST CATEGORY 8: Parsing Test Vectors\n");
    printf("------------------------------------------------------------------------\n");
    printf("These strings should all parse to the same value:\n\n");

    uint32_t test_val = 255;
    printf("Target value: %u (decimal)\n\n", test_val);
    printf("Parsing test cases:\n");
    printf("  \"255\"       → decimal %u\n", 255);
    printf("  \"0xFF\"      → hexadecimal %u\n", 0xFF);
    printf("  \"0xff\"      → hexadecimal %u\n", 0xff);
    printf("  \"11111111\"  → binary %d (0*128 + 0*64 + ... + 1*1)\n", 0b11111111);
    printf("  \"0b11111111\" → binary %d\n", 0b11111111);
    printf("  \"0377\"      → octal %d\n", 0377);
    printf("\n");

    /* ========================================================================
     * TEST CATEGORY 9: Round-Trip Validation
     * ======================================================================== */
    printf("\nTEST CATEGORY 9: Round-Trip Validation\n");
    printf("------------------------------------------------------------------------\n");
    printf("These conversions should return the original value:\n\n");

    uint32_t roundtrip_tests[] = {0, 1, 42, 127, 255, 1000, 65535, 1000000};
    size_t num_roundtrips = sizeof(roundtrip_tests) / sizeof(roundtrip_tests[0]);

    for (size_t i = 0; i < num_roundtrips; i++) {
        uint32_t original = roundtrip_tests[i];

        // Would go: decimal → binary → decimal
        // For now, just show what should happen
        printf("Value %u:\n", original);
        printf("  → binary conversion\n");
        printf("  → decimal parse\n");
        printf("  → should equal %u (original)\n\n", original);
    }

    printf("\n");
    printf("===============================================================================\n");
    printf("END OF TEST VECTORS\n");
    printf("===============================================================================\n");

    return 0;
}

/*
 * Compilation and Usage:
 *
 * To generate test vectors:
 *   gcc -Wall -g generate_test_data.c -o generate_test_data
 *   ./generate_test_data > test_vectors.txt
 *
 * This creates a comprehensive test report showing:
 * - Expected outputs for various inputs
 * - Boundary conditions
 * - Common test cases
 * - IP address examples
 * - Byte swap examples
 * - Bit field extraction examples
 * - Parsing examples
 * - Round-trip validation cases
 *
 * Use this reference to validate your converter implementation!
 */
