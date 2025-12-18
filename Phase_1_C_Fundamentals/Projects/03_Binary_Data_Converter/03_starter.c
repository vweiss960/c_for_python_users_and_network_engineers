/*
 * Binary Data Converter - Starter Template
 *
 * This program converts numbers between different bases (decimal, binary,
 * hexadecimal, octal) and handles network data transformations.
 *
 * Complete this file by implementing the TODO sections.
 * Follow the implementation guide in PROJECT_GUIDE.md for step-by-step instructions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <ctype.h>

/*
 * ============================================================================
 * PART 1: Type Definitions and Constants
 * ============================================================================
 */

// Maximum buffer sizes
#define BINARY_STR_MAX 33      // 32 bits + null terminator
#define HEX_STR_MAX 9          // 8 hex digits + null terminator
#define OCTAL_STR_MAX 12       // 11 octal digits + null terminator
#define IP_STR_MAX 16          // "255.255.255.255" + null terminator

// Conversion result codes
typedef enum {
    RESULT_OK = 0,
    RESULT_INVALID_INPUT = 1,
    RESULT_OVERFLOW = 2,
    RESULT_FORMAT_ERROR = 3
} ConversionResult;

// Input format types
typedef enum {
    FORMAT_UNKNOWN,
    FORMAT_DECIMAL,
    FORMAT_BINARY,
    FORMAT_HEX,
    FORMAT_OCTAL
} InputFormat;

// Output formats requested
typedef enum {
    OUTPUT_DECIMAL = 1 << 0,
    OUTPUT_BINARY = 1 << 1,
    OUTPUT_HEX = 1 << 2,
    OUTPUT_OCTAL = 1 << 3,
    OUTPUT_ALL = OUTPUT_DECIMAL | OUTPUT_BINARY | OUTPUT_HEX | OUTPUT_OCTAL
} OutputFormat;

/*
 * ============================================================================
 * PART 2: Base Conversion Functions
 * ============================================================================
 * These functions convert between different number bases.
 * Start with these - they are the foundation for everything else.
 */

/*
 * Convert unsigned integer to binary string representation.
 *
 * Parameters:
 *   value: The unsigned integer to convert (0-255 for 8-bit example)
 *   buffer: Character buffer to store the binary string
 *   buffer_size: Size of the buffer (must be at least 33 for full 32-bit)
 *
 * Returns:
 *   RESULT_OK on success
 *
 * Example:
 *   format_binary(42, buffer, 33) → buffer contains "00101010"
 *
 * TODO:
 *   1. Iterate through bit positions from 31 (MSB) down to 0 (LSB)
 *   2. Extract each bit using: (value >> i) & 1
 *   3. Convert bit to character ('0' or '1')
 *   4. Store in buffer
 *   5. Add null terminator
 */
ConversionResult format_binary(uint32_t value, char *buffer, size_t buffer_size) {
    if (buffer_size < BINARY_STR_MAX) {
        return RESULT_FORMAT_ERROR;
    }

    // TODO: Implement binary conversion here
    // Hint: Use a loop from bit 31 down to 0
    // Hint: buffer[index] = ((value >> bit) & 1) ? '1' : '0'

    return RESULT_OK;
}

/*
 * Convert unsigned integer to hexadecimal string representation.
 *
 * Parameters:
 *   value: The unsigned integer to convert
 *   buffer: Character buffer for the hex string
 *   buffer_size: Size of buffer (must be at least 9 for 32-bit)
 *
 * Example:
 *   format_hex(255, buffer, 9) → buffer contains "000000ff"
 *
 * TODO:
 *   1. Create hex digit lookup: "0123456789abcdef"
 *   2. Extract 4 bits at a time (positions 28, 24, 20, ..., 0)
 *   3. Map each 4-bit group to a hex digit using lookup
 *   4. Build string from left to right
 */
ConversionResult format_hex(uint32_t value, char *buffer, size_t buffer_size) {
    if (buffer_size < HEX_STR_MAX) {
        return RESULT_FORMAT_ERROR;
    }

    // TODO: Implement hex conversion here
    // Hint: const char *hex_digits = "0123456789abcdef";
    // Hint: Extract (value >> position) & 0xF

    return RESULT_OK;
}

/*
 * Convert unsigned integer to octal string representation.
 *
 * Parameters:
 *   value: The unsigned integer to convert
 *   buffer: Character buffer for the octal string
 *   buffer_size: Size of buffer
 *
 * Example:
 *   format_octal(255, buffer, 12) → buffer contains "000000377"
 *
 * TODO:
 *   1. Extract 3 bits at a time (positions 30, 27, 24, ..., 0)
 *   2. Convert each 3-bit group to octal digit (0-7)
 *   3. Build string
 */
ConversionResult format_octal(uint32_t value, char *buffer, size_t buffer_size) {
    if (buffer_size < OCTAL_STR_MAX) {
        return RESULT_FORMAT_ERROR;
    }

    // TODO: Implement octal conversion here
    // Hint: Extract (value >> position) & 0x7

    return RESULT_OK;
}

/*
 * Parse binary string ("00101010") to unsigned integer (42).
 *
 * Parameters:
 *   str: Binary string containing only '0' and '1' characters
 *
 * Returns:
 *   Parsed value, or 0 on error
 *
 * Example:
 *   parse_binary("00101010") → 42
 *   parse_binary("invalid") → 0
 *
 * TODO:
 *   1. Validate all characters are '0' or '1'
 *   2. For each character, shift left and add new bit
 *   3. Return result
 */
uint32_t parse_binary(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    // TODO: Implement binary parsing here
    // Hint: result = (result << 1) | (char - '0')

    return 0;  // Placeholder - replace with your implementation
}

/*
 * Parse hexadecimal string ("FF" or "0xFF") to unsigned integer.
 *
 * Parameters:
 *   str: Hex string, optionally starting with "0x" or "0X"
 *
 * Returns:
 *   Parsed value, or 0 on error
 *
 * Example:
 *   parse_hex("FF") → 255
 *   parse_hex("0xFF") → 255
 *   parse_hex("deadbeef") → 0xDEADBEEF
 *
 * TODO:
 *   Use strtoul with base 16 for simplicity, or implement manually
 */
uint32_t parse_hex(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    // TODO: Use strtoul(str, NULL, 16) or manual parsing
    // Hint: strtoul handles "0x" prefix automatically

    return 0;  // Placeholder
}

/*
 * Parse octal string ("377" or "0377") to unsigned integer.
 *
 * Parameters:
 *   str: Octal string, optionally starting with "0"
 *
 * Returns:
 *   Parsed value, or 0 on error
 *
 * Example:
 *   parse_octal("377") → 255
 *   parse_octal("0377") → 255
 */
uint32_t parse_octal(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    // TODO: Use strtoul(str, NULL, 8) for simplicity

    return 0;  // Placeholder
}

/*
 * Auto-detect input format and parse accordingly.
 *
 * Detects format by prefix:
 *   "0x..." or "0X..." → hexadecimal
 *   "0b..." or "0B..." → binary
 *   "0..." (leading zero) → octal
 *   Otherwise → decimal
 *
 * TODO:
 *   Check first 1-2 characters to determine format
 *   Call appropriate parsing function
 */
uint32_t parse_auto(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    // TODO: Implement format detection and parsing
    // Hint: Check str[0] and str[1] for format indicators

    return 0;  // Placeholder
}

/*
 * ============================================================================
 * PART 3: Endianness Functions
 * ============================================================================
 * These functions handle byte order (big-endian vs little-endian).
 */

/*
 * Detect system's native byte order.
 *
 * Returns:
 *   1 if system is big-endian
 *   0 if system is little-endian
 *
 * Algorithm:
 *   Create test value 0x12345678
 *   Check first byte in memory
 *   If first byte is 0x12 → big-endian
 *   If first byte is 0x78 → little-endian
 *
 * TODO:
 *   Create 0x12345678, cast to unsigned char pointer
 *   Check bytes[0]
 */
int detect_endianness(void) {
    // TODO: Implement endianness detection

    return 0;  // Placeholder
}

/*
 * Manually swap bytes in 32-bit value.
 *
 * Parameters:
 *   value: Value to byte-swap
 *
 * Returns:
 *   Value with bytes reversed
 *
 * Example:
 *   swap_bytes_32(0x12345678) → 0x78563412
 *
 * Algorithm:
 *   Extract each byte, shift to opposite position
 *   Byte 0 (LSB) → Byte 3 position
 *   Byte 1 → Byte 2 position
 *   Byte 2 → Byte 1 position
 *   Byte 3 (MSB) → Byte 0 position
 *
 * TODO:
 *   Implement byte swapping using bitwise operations
 */
uint32_t swap_bytes_32(uint32_t value) {
    // TODO: Implement 32-bit byte swap
    // Hint: Extract with & 0x[FF]000000, shift to new position

    return 0;  // Placeholder
}

/*
 * Manually swap bytes in 16-bit value.
 *
 * Example:
 *   swap_bytes_16(0x1234) → 0x3412
 */
uint16_t swap_bytes_16(uint16_t value) {
    // TODO: Implement 16-bit byte swap
    // Hint: Simpler than 32-bit version

    return 0;  // Placeholder
}

/*
 * ============================================================================
 * PART 4: Network Data Functions
 * ============================================================================
 * These functions handle network protocols (IP, ports, etc.)
 */

/*
 * Parse IP address string ("192.168.1.1") to binary form.
 *
 * Parameters:
 *   ip_str: IP address string in dotted decimal notation
 *
 * Returns:
 *   32-bit IP address in network byte order
 *   0 on invalid input
 *
 * Example:
 *   parse_ip_string("192.168.1.1") → 0xC0A80101
 *
 * Algorithm:
 *   Parse four octets using sscanf with "%hhu"
 *   Combine into 32-bit value: (a<<24) | (b<<16) | (c<<8) | d
 *   Convert to network byte order with htonl()
 *
 * TODO:
 *   Use sscanf to parse four unsigned char values
 *   Validate that all four values were parsed (sscanf returns 4)
 *   Combine and convert to network byte order
 */
uint32_t parse_ip_string(const char *ip_str) {
    if (ip_str == NULL || ip_str[0] == '\0') {
        return 0;
    }

    // TODO: Implement IP address parsing
    // Hint: unsigned char a, b, c, d;
    // Hint: sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d)

    return 0;  // Placeholder
}

/*
 * Format IP address from binary form to string.
 *
 * Parameters:
 *   ip_binary: 32-bit IP address in network byte order
 *   buffer: Character buffer for result
 *   buffer_size: Size of buffer (should be at least IP_STR_MAX)
 *
 * Example:
 *   format_ip_address(0xC0A80101, buffer, IP_STR_MAX);
 *   buffer contains "192.168.1.1"
 *
 * Algorithm:
 *   Convert from network byte order with ntohl()
 *   Extract four octets using bit shifting and masking
 *   Format as string using snprintf()
 *
 * TODO:
 *   Convert from network byte order
 *   Extract each octet: (ip >> (24-8*i)) & 0xFF
 *   Format to string
 */
void format_ip_address(uint32_t ip_binary, char *buffer, size_t buffer_size) {
    if (buffer_size < IP_STR_MAX) {
        return;
    }

    // TODO: Implement IP address formatting
    // Hint: ntohl() to convert byte order
    // Hint: Extract bytes with (ip >> 24) & 0xFF, etc.

    buffer[0] = '\0';  // Placeholder
}

/*
 * Get service name for well-known port number.
 *
 * Parameters:
 *   port: Port number (host byte order)
 *
 * Returns:
 *   Pointer to service name string, or "Unknown" for unknown ports
 *
 * Common ports:
 *   21 → FTP
 *   22 → SSH
 *   25 → SMTP
 *   53 → DNS
 *   80 → HTTP
 *   110 → POP3
 *   143 → IMAP
 *   443 → HTTPS
 *   3306 → MySQL
 *   5432 → PostgreSQL
 *
 * TODO:
 *   Implement switch statement or lookup table
 */
const char* get_port_name(uint16_t port) {
    // TODO: Implement port name lookup
    // Hint: Use switch statement with common port numbers

    return "Unknown";
}

/*
 * ============================================================================
 * PART 5: Bit-Level Operations
 * ============================================================================
 * These functions work with individual bits and bit fields.
 */

/*
 * Extract N bits starting at position start_bit.
 *
 * Parameters:
 *   value: The value to extract from
 *   start_bit: Position of the LSB of the field (0-31)
 *   num_bits: Number of bits to extract (1-32)
 *
 * Returns:
 *   Extracted field value (0 to 2^num_bits - 1)
 *
 * Example:
 *   extract_field(0b11010110, 3, 3) → 5 (bits [5:3] = 0b101)
 *
 * Algorithm:
 *   Create mask with num_bits ones: (1 << num_bits) - 1
 *   Shift right by start_bit: (value >> start_bit)
 *   AND with mask: & mask
 *
 * TODO:
 *   Implement mask creation and extraction
 */
uint32_t extract_field(uint32_t value, int start_bit, int num_bits) {
    // TODO: Implement field extraction
    // Hint: Create mask: (1U << num_bits) - 1

    return 0;  // Placeholder
}

/*
 * Set N bits starting at position start_bit to a new value.
 *
 * Parameters:
 *   value: Pointer to value to modify (IN/OUT)
 *   start_bit: Position of the LSB of the field
 *   num_bits: Number of bits to set
 *   field_value: New value for the field
 *
 * Example:
 *   set_field(&data, 3, 3, 5) sets bits [5:3] to 0b101
 *
 * Algorithm:
 *   Create mask: (1 << num_bits) - 1
 *   Clear existing bits: *value &= ~(mask << start_bit)
 *   Set new bits: *value |= (field_value & mask) << start_bit
 *
 * TODO:
 *   Implement field modification
 */
void set_field(uint32_t *value, int start_bit, int num_bits,
               uint32_t field_value) {
    // TODO: Implement field setting
    // Hint: Clear bits first, then set new value
}

/*
 * ============================================================================
 * PART 6: Output and Formatting Functions
 * ============================================================================
 */

/*
 * Display all conversions for a given value.
 *
 * TODO:
 *   Call format_binary, format_hex, format_octal
 *   Print results in nice format
 */
void display_all_conversions(uint32_t value) {
    char binary[BINARY_STR_MAX];
    char hex[HEX_STR_MAX];
    char octal[OCTAL_STR_MAX];

    format_binary(value, binary, sizeof(binary));
    format_hex(value, hex, sizeof(hex));
    format_octal(value, octal, sizeof(octal));

    printf("=== Binary Data Converter ===\n");
    printf("Input value: %u (decimal)\n\n", value);
    printf("Conversions:\n");
    printf("  Decimal:      %u\n", value);
    printf("  Binary:       %s\n", binary);
    printf("  Hexadecimal:  0x%s\n", hex);
    printf("  Octal:        0%s\n", octal);
}

/*
 * Display IP address in multiple formats.
 *
 * TODO:
 *   Parse IP string
 *   Display in binary, hex, decimal formats
 */
void display_ip_conversions(const char *ip_str) {
    uint32_t ip = parse_ip_string(ip_str);
    if (ip == 0) {
        printf("Invalid IP address: %s\n", ip_str);
        return;
    }

    // TODO: Implement IP display with multiple formats
}

/*
 * Print usage information.
 */
void print_usage(void) {
    printf("Binary Data Converter - Usage\n\n");
    printf("Syntax:  converter <value> [format]\n\n");
    printf("Formats:\n");
    printf("  all              Show all conversions\n");
    printf("  binary           Binary representation only\n");
    printf("  hex              Hexadecimal only\n");
    printf("  octal            Octal only\n");
    printf("  ip               IP address format\n");
    printf("  ip-binary        IP in binary\n");
    printf("  port             Port number information\n");
    printf("  endian           Byte order conversions\n");
    printf("  swap             Swap bytes\n");
    printf("  detect-endian    Detect system endianness\n\n");
    printf("Examples:\n");
    printf("  converter 255 all\n");
    printf("  converter 192.168.1.1 ip\n");
    printf("  converter 0xDEADBEEF swap\n");
    printf("  converter detect-endian\n");
}

/*
 * ============================================================================
 * PART 7: Main Program
 * ============================================================================
 * This is where everything comes together.
 */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    const char *input_str = argv[1];
    const char *format_str = (argc > 2) ? argv[2] : "all";

    // TODO: Implement main program logic
    // 1. Parse input value (auto-detect format)
    // 2. Determine output format from second argument
    // 3. Dispatch to appropriate display function
    // 4. Handle special cases (detect-endian, ip addresses, etc.)

    // Placeholder - show all conversions for any decimal number
    uint32_t value = atoi(input_str);

    if (strcmp(format_str, "all") == 0) {
        display_all_conversions(value);
    } else if (strcmp(format_str, "detect-endian") == 0) {
        // TODO: Implement endianness detection
        printf("System endianness detection not yet implemented\n");
    } else if (strcmp(format_str, "ip") == 0) {
        // TODO: Implement IP address display
        display_ip_conversions(input_str);
    } else {
        printf("Unknown format: %s\n", format_str);
        return 1;
    }

    return 0;
}

/*
 * ============================================================================
 * HELPFUL NOTES
 * ============================================================================
 *
 * When you get stuck:
 *
 * 1. BINARY CONVERSION:
 *    - Iterate from MSB (bit 31) down to LSB (bit 0)
 *    - Use (value >> i) & 1 to extract each bit
 *    - Convert to '0' or '1' character
 *
 * 2. BITWISE OPERATIONS:
 *    - Shift right then mask: (value >> position) & mask
 *    - Create mask with ones: (1U << num_bits) - 1
 *    - Never shift by values >= 32 for 32-bit integer
 *
 * 3. ENDIANNESS:
 *    - Network byte order = big-endian (MSB first)
 *    - Use htonl/ntohl to convert 32-bit values
 *    - Use htons/ntohs to convert 16-bit values
 *
 * 4. IP ADDRESSES:
 *    - Parse with sscanf("%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d)
 *    - Check that sscanf returns 4 (all fields parsed)
 *    - Always use htonl/ntohl for byte order
 *
 * 5. TESTING:
 *    - Use known values: 0, 1, 255, powers of 2
 *    - Test round-trip: decimal → binary → decimal should equal original
 *    - Test edge cases: 0, max values, invalid input
 *
 * Reference implementation: 03_c_solution.c
 * Detailed explanation: 03_c_solution_explained.md
 * Implementation guide: PROJECT_GUIDE.md
 */
