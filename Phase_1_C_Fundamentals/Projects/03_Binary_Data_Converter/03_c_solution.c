/*
 * Binary Data Converter - Reference Solution
 *
 * This is a complete, working implementation of the binary converter.
 * Compare your solution with this to understand the correct approach.
 *
 * Study this code after attempting the starter template!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BINARY_STR_MAX 33
#define HEX_STR_MAX 9
#define OCTAL_STR_MAX 12
#define IP_STR_MAX 16

typedef enum {
    RESULT_OK = 0,
    RESULT_INVALID_INPUT = 1,
    RESULT_OVERFLOW = 2,
    RESULT_FORMAT_ERROR = 3
} ConversionResult;

typedef enum {
    OUTPUT_DECIMAL = 1 << 0,
    OUTPUT_BINARY = 1 << 1,
    OUTPUT_HEX = 1 << 2,
    OUTPUT_OCTAL = 1 << 3,
    OUTPUT_ALL = OUTPUT_DECIMAL | OUTPUT_BINARY | OUTPUT_HEX | OUTPUT_OCTAL
} OutputFormat;

/* ============================================================================
 * BASE CONVERSION FUNCTIONS
 * ============================================================================ */

ConversionResult format_binary(uint32_t value, char *buffer, size_t buffer_size) {
    if (buffer_size < BINARY_STR_MAX) {
        return RESULT_FORMAT_ERROR;
    }

    // Process all 32 bits, starting from MSB (bit 31) down to LSB (bit 0)
    for (int i = 31; i >= 0; i--) {
        // Extract bit at position i and convert to character
        int bit = (value >> i) & 1;
        buffer[31 - i] = bit ? '1' : '0';
    }
    buffer[32] = '\0';  // Null terminate

    return RESULT_OK;
}

ConversionResult format_hex(uint32_t value, char *buffer, size_t buffer_size) {
    if (buffer_size < HEX_STR_MAX) {
        return RESULT_FORMAT_ERROR;
    }

    const char *hex_digits = "0123456789abcdef";
    int index = 0;

    // Extract 4 bits at a time, starting from position 28 down to 0
    for (int i = 28; i >= 0; i -= 4) {
        // Extract 4 bits (one hex digit)
        int digit = (value >> i) & 0xF;
        buffer[index++] = hex_digits[digit];
    }
    buffer[index] = '\0';

    return RESULT_OK;
}

ConversionResult format_octal(uint32_t value, char *buffer, size_t buffer_size) {
    if (buffer_size < OCTAL_STR_MAX) {
        return RESULT_FORMAT_ERROR;
    }

    int index = 0;

    // Extract 3 bits at a time, starting from position 30 down to 0
    // Note: We process 30,27,24,... but pad with leading zeros for 32 bits
    for (int i = 30; i >= 0; i -= 3) {
        // Extract 3 bits (one octal digit)
        int digit = (value >> i) & 0x7;
        buffer[index++] = '0' + digit;
    }
    buffer[index] = '\0';

    return RESULT_OK;
}

uint32_t parse_binary(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    // Skip "0b" or "0B" prefix if present
    if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
        str += 2;
    }

    uint32_t result = 0;

    // Process each character
    for (int i = 0; str[i] != '\0'; i++) {
        // Validate character
        if (str[i] != '0' && str[i] != '1') {
            return 0;  // Invalid binary digit
        }

        // Shift previous bits left and add new bit
        result = (result << 1) | (str[i] - '0');
    }

    return result;
}

uint32_t parse_hex(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    // Use strtoul for hex parsing - handles "0x" prefix
    char *endptr;
    uint32_t value = (uint32_t)strtoul(str, &endptr, 16);

    // Validate that entire string was parsed
    if (*endptr != '\0') {
        return 0;  // Invalid characters in string
    }

    return value;
}

uint32_t parse_octal(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    // Use strtoul for octal parsing
    char *endptr;
    uint32_t value = (uint32_t)strtoul(str, &endptr, 8);

    // Validate entire string was parsed
    if (*endptr != '\0') {
        return 0;  // Invalid characters
    }

    return value;
}

uint32_t parse_auto(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    // Detect format by prefix
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        return parse_hex(str);  // Hex: "0x..."
    } else if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
        return parse_binary(str + 2);  // Binary: "0b..."
    } else if (str[0] == '0' && isdigit(str[1]) && str[1] >= '0' && str[1] <= '7') {
        return parse_octal(str);  // Octal: "0..."
    } else {
        return strtoul(str, NULL, 10);  // Decimal: default
    }
}

/* ============================================================================
 * ENDIANNESS FUNCTIONS
 * ============================================================================ */

int detect_endianness(void) {
    uint32_t test = 0x12345678;
    unsigned char *bytes = (unsigned char *)&test;

    // Big-endian: first byte is 0x12 (MSB)
    // Little-endian: first byte is 0x78 (LSB)
    return bytes[0] == 0x12;
}

uint32_t swap_bytes_32(uint32_t value) {
    // Extract each byte and shift to opposite position
    uint32_t result = 0;
    result |= (value & 0x000000FF) << 24;  // Byte 0 → Byte 3
    result |= (value & 0x0000FF00) << 8;   // Byte 1 → Byte 2
    result |= (value & 0x00FF0000) >> 8;   // Byte 2 → Byte 1
    result |= (value & 0xFF000000) >> 24;  // Byte 3 → Byte 0
    return result;
}

uint16_t swap_bytes_16(uint16_t value) {
    return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

/* ============================================================================
 * NETWORK DATA FUNCTIONS
 * ============================================================================ */

uint32_t parse_ip_string(const char *ip_str) {
    if (ip_str == NULL || ip_str[0] == '\0') {
        return 0;
    }

    unsigned char a, b, c, d;

    // Parse four octets separated by dots
    if (sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d) != 4) {
        return 0;  // Invalid format
    }

    // Combine into 32-bit value in host byte order
    uint32_t ip = (a << 24) | (b << 16) | (c << 8) | d;

    // Convert to network byte order (big-endian)
    return htonl(ip);
}

void format_ip_address(uint32_t ip_binary, char *buffer, size_t buffer_size) {
    if (buffer_size < IP_STR_MAX) {
        return;
    }

    // Convert from network byte order to host byte order
    uint32_t host_ip = ntohl(ip_binary);

    // Extract individual octets
    unsigned char a = (host_ip >> 24) & 0xFF;
    unsigned char b = (host_ip >> 16) & 0xFF;
    unsigned char c = (host_ip >> 8) & 0xFF;
    unsigned char d = host_ip & 0xFF;

    // Format as string
    snprintf(buffer, buffer_size, "%u.%u.%u.%u", a, b, c, d);
}

const char* get_port_name(uint16_t port) {
    switch (port) {
        case 21:    return "FTP";
        case 22:    return "SSH";
        case 25:    return "SMTP";
        case 53:    return "DNS";
        case 80:    return "HTTP";
        case 110:   return "POP3";
        case 143:   return "IMAP";
        case 443:   return "HTTPS";
        case 3306:  return "MySQL";
        case 5432:  return "PostgreSQL";
        case 8080:  return "HTTP-alt";
        default:    return "Unknown";
    }
}

/* ============================================================================
 * BIT-LEVEL OPERATIONS
 * ============================================================================ */

uint32_t extract_field(uint32_t value, int start_bit, int num_bits) {
    // Create mask with num_bits ones
    uint32_t mask = (1U << num_bits) - 1;

    // Shift right to position, then mask
    return (value >> start_bit) & mask;
}

void set_field(uint32_t *value, int start_bit, int num_bits,
               uint32_t field_value) {
    // Create mask with num_bits ones
    uint32_t mask = (1U << num_bits) - 1;

    // Clear the bits we're about to set
    *value &= ~(mask << start_bit);

    // Set new bits (mask the input in case it's too large)
    *value |= (field_value & mask) << start_bit;
}

/* ============================================================================
 * OUTPUT AND FORMATTING
 * ============================================================================ */

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

void display_ip_conversions(const char *ip_str) {
    uint32_t ip = parse_ip_string(ip_str);
    if (ip == 0 && strcmp(ip_str, "0.0.0.0") != 0) {
        printf("Invalid IP address: %s\n", ip_str);
        return;
    }

    char ip_back[IP_STR_MAX];
    format_ip_address(ip, ip_back, sizeof(ip_back));

    char binary[BINARY_STR_MAX];
    char hex[HEX_STR_MAX];

    format_binary(ntohl(ip), binary, sizeof(binary));
    format_hex(ntohl(ip), hex, sizeof(hex));

    printf("=== IP Address Converter ===\n");
    printf("Input: %s\n\n", ip_str);
    printf("Representations:\n");
    printf("  String:       %s\n", ip_back);
    printf("  Hexadecimal:  0x%s\n", hex);
    printf("  Binary:       ");

    // Print binary with separators for clarity
    for (int i = 0; i < 32; i++) {
        if (i > 0 && i % 8 == 0) printf(" ");
        printf("%c", binary[i]);
    }
    printf("\n\n");

    // Print octets
    printf("Octets:\n");
    unsigned char a, b, c, d;
    sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d);
    printf("  %u = 0x%02x = %s\n", a, a, binary);
    printf("  %u = 0x%02x = %s\n", b, b, binary + 8);
    printf("  %u = 0x%02x = %s\n", c, c, binary + 16);
    printf("  %u = 0x%02x = %s\n", d, d, binary + 24);
}

void display_endianness_info(void) {
    printf("=== System Endianness ===\n");
    printf("Endianness: %s\n", detect_endianness() ? "big-endian" : "little-endian");

    uint32_t test = 0x12345678;
    unsigned char *bytes = (unsigned char *)&test;

    printf("Test value: 0x12345678\n");
    printf("Bytes in memory: ");
    for (int i = 0; i < 4; i++) {
        printf("%02x", bytes[i]);
        if (i < 3) printf(" ");
    }
    printf("\n\n");

    if (detect_endianness()) {
        printf("Big-endian (Motorola style):\n");
        printf("  Most significant byte stored first\n");
        printf("  Used in network protocols (network byte order)\n");
    } else {
        printf("Little-endian (Intel/ARM style):\n");
        printf("  Least significant byte stored first\n");
        printf("  Common in modern systems\n");
    }
}

void display_byte_swap(uint32_t value) {
    char hex_orig[HEX_STR_MAX];
    char hex_swap[HEX_STR_MAX];

    format_hex(value, hex_orig, sizeof(hex_orig));
    format_hex(swap_bytes_32(value), hex_swap, sizeof(hex_swap));

    printf("=== Byte Swap ===\n");
    printf("Original value: 0x%s\n", hex_orig);
    printf("Swapped value:  0x%s\n", hex_swap);

    unsigned char *bytes_orig = (unsigned char *)&value;
    uint32_t swapped = swap_bytes_32(value);
    unsigned char *bytes_swap = (unsigned char *)&swapped;

    printf("\nByte representation:\n");
    printf("  Original: ");
    for (int i = 0; i < 4; i++) {
        printf("%02x", bytes_orig[i]);
        if (i < 3) printf(" ");
    }
    printf("\n");

    printf("  Swapped:  ");
    for (int i = 0; i < 4; i++) {
        printf("%02x", bytes_swap[i]);
        if (i < 3) printf(" ");
    }
    printf("\n");
}

void print_usage(void) {
    printf("Binary Data Converter - Usage\n\n");
    printf("Syntax:  converter <value> [format]\n\n");
    printf("Formats:\n");
    printf("  all              Show all conversions (default)\n");
    printf("  binary           Binary representation only\n");
    printf("  hex              Hexadecimal only\n");
    printf("  octal            Octal only\n");
    printf("  ip               IP address format\n");
    printf("  ip-binary        IP in binary\n");
    printf("  endian           Byte order conversions\n");
    printf("  swap             Swap bytes\n");
    printf("  detect-endian    Detect system endianness\n\n");
    printf("Examples:\n");
    printf("  converter 255 all\n");
    printf("  converter 0xFF all\n");
    printf("  converter 0b11111111 all\n");
    printf("  converter 192.168.1.1 ip\n");
    printf("  converter 0xDEADBEEF swap\n");
    printf("  converter detect-endian\n");
}

/* ============================================================================
 * MAIN PROGRAM
 * ============================================================================ */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    const char *input_str = argv[1];
    const char *format_str = (argc > 2) ? argv[2] : "all";

    // Handle special cases first
    if (strcmp(format_str, "detect-endian") == 0 ||
        strcmp(input_str, "detect-endian") == 0) {
        display_endianness_info();
        return 0;
    }

    // Check if input is an IP address
    if (strchr(input_str, '.') != NULL) {
        // IP address format
        if (strcmp(format_str, "ip") == 0 || strcmp(format_str, "all") == 0) {
            display_ip_conversions(input_str);
        } else {
            printf("Unknown format for IP address: %s\n", format_str);
            return 1;
        }
        return 0;
    }

    // Parse numeric input (auto-detect base)
    uint32_t value = parse_auto(input_str);

    // Handle various output formats
    if (strcmp(format_str, "all") == 0) {
        display_all_conversions(value);
    } else if (strcmp(format_str, "binary") == 0) {
        char buffer[BINARY_STR_MAX];
        format_binary(value, buffer, sizeof(buffer));
        printf("Binary: %s\n", buffer);
    } else if (strcmp(format_str, "hex") == 0) {
        char buffer[HEX_STR_MAX];
        format_hex(value, buffer, sizeof(buffer));
        printf("Hexadecimal: 0x%s\n", buffer);
    } else if (strcmp(format_str, "octal") == 0) {
        char buffer[OCTAL_STR_MAX];
        format_octal(value, buffer, sizeof(buffer));
        printf("Octal: 0%s\n", buffer);
    } else if (strcmp(format_str, "swap") == 0) {
        display_byte_swap(value);
    } else if (strcmp(format_str, "endian") == 0) {
        display_endianness_info();
    } else {
        printf("Unknown format: %s\n", format_str);
        print_usage();
        return 1;
    }

    return 0;
}
