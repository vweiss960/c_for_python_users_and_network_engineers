#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  // For htons(), ntohs(), htonl(), ntohl()

// ============================================================================
// IMPORTANT: Use #pragma pack(push, 1) to remove struct padding!
// Network packets are tightly packed - no alignment padding allowed
// ============================================================================

#pragma pack(push, 1)

// IPv4 Header Structure (20 bytes minimum)
// Reference: RFC 791
// Bits:  0-3: Version, 4-7: IHL
//        8-13: DSCP, 14-15: ECN
//        16-31: Total Length
//        32-47: Identification
//        48-50: Flags (3 bits), 51-63: Fragment Offset (13 bits)
//        64-71: TTL
//        72-79: Protocol
//        80-95: Header Checksum
//        96-127: Source IP Address
//        128-159: Destination IP Address
typedef struct {
    unsigned char version_ihl;      // Version (4 bits) + IHL (4 bits)
    unsigned char dscp_ecn;         // DSCP (6 bits) + ECN (2 bits)
    unsigned short total_length;    // Total packet length (including payload)
    unsigned short identification;  // Identification
    unsigned short flags_offset;    // Flags (3 bits) + Fragment Offset (13 bits)
    unsigned char ttl;              // Time to Live
    unsigned char protocol;         // Protocol (6=TCP, 17=UDP, etc.)
    unsigned short header_checksum; // Header checksum
    unsigned int source_ip;         // Source IP address (network byte order)
    unsigned int dest_ip;           // Destination IP address (network byte order)
} IPv4Header;

// TCP Header Structure (20 bytes minimum)
// Reference: RFC 793
typedef struct {
    unsigned short source_port;     // Source port
    unsigned short dest_port;       // Destination port
    unsigned int sequence_num;      // Sequence number
    unsigned int ack_num;           // Acknowledgment number
    unsigned char data_offset;      // Data offset (4 bits) + Reserved (4 bits)
    unsigned char flags;            // FIN, SYN, RST, PSH, ACK, URG flags
    unsigned short window_size;     // Window size
    unsigned short checksum;        // Checksum
    unsigned short urgent_pointer;  // Urgent pointer
} TCPHeader;

// UDP Header Structure (8 bytes)
// Reference: RFC 768
typedef struct {
    unsigned short source_port;     // Source port
    unsigned short dest_port;       // Destination port
    unsigned short length;          // Length of UDP header + payload
    unsigned short checksum;        // Checksum
} UDPHeader;

#pragma pack(pop)

// ============================================================================
// BIT EXTRACTION HELPER FUNCTIONS
// ============================================================================

// Extract version field from first byte of IP header (top 4 bits)
unsigned char get_ip_version(unsigned char version_ihl) {
    // TODO: Extract top 4 bits using bitwise operations
    // Hint: Use right shift (>>) and AND (&) mask
    return 0;
}

// Extract IHL field from first byte of IP header (bottom 4 bits)
unsigned char get_ihl(unsigned char version_ihl) {
    // TODO: Extract bottom 4 bits using bitwise operations
    return 0;
}

// Extract DSCP field from second byte (top 6 bits)
unsigned char get_dscp(unsigned char dscp_ecn) {
    // TODO: Extract top 6 bits using bitwise operations
    return 0;
}

// Extract ECN field from second byte (bottom 2 bits)
unsigned char get_ecn(unsigned char dscp_ecn) {
    // TODO: Extract bottom 2 bits using bitwise operations
    return 0;
}

// Extract flags from flags_offset field (top 3 bits)
void get_ip_flags(unsigned short flags_offset, int *more_fragments, int *dont_fragment, int *reserved) {
    // TODO: Extract the 3 flag bits from the top 3 bits of this 16-bit field
    // Remember to convert from network byte order!
    // Flags are: Bit 0 = Reserved (always 0), Bit 1 = DF, Bit 2 = MF
    *more_fragments = 0;
    *dont_fragment = 0;
    *reserved = 0;
}

// Extract fragment offset from flags_offset field (bottom 13 bits)
unsigned short get_fragment_offset(unsigned short flags_offset) {
    // TODO: Extract bottom 13 bits from flags_offset field
    return 0;
}

// Extract TCP flags from flags byte
void get_tcp_flags(unsigned char flags,
                   int *fin, int *syn, int *rst,
                   int *psh, int *ack, int *urg) {
    // TODO: Extract individual flag bits from the flags byte
    // Bit order: FIN(0), SYN(1), RST(2), PSH(3), ACK(4), URG(5)
    // Use: flag = (byte >> bit_position) & 1
    *fin = 0;
    *syn = 0;
    *rst = 0;
    *psh = 0;
    *ack = 0;
    *urg = 0;
}

// Extract TCP data offset (4 bits, in words = 4-byte units)
unsigned char get_tcp_data_offset(unsigned char data_offset) {
    // TODO: Extract top 4 bits of data_offset byte
    return 0;
}

// ============================================================================
// IP ADDRESS FORMATTING
// ============================================================================

// Convert 4-byte IP address (network byte order) to dotted decimal string
void format_ip_address(unsigned int ip_bytes, char *buffer, size_t buffer_size) {
    // TODO: Convert network byte order IP to human-readable format
    // Example: 0xC0A80001 -> "192.168.0.1"
    // Hint: Cast to unsigned char* and extract individual bytes
    // Remember byte order!

    snprintf(buffer, buffer_size, "0.0.0.0");  // Placeholder
}

// ============================================================================
// PORT NUMBER LOOKUP
// ============================================================================

// Lookup common port names
const char* get_port_name(unsigned short port) {
    // TODO: Return common port names for well-known ports
    // Examples: 80->HTTP, 443->HTTPS, 22->SSH, 21->FTP, 53->DNS, 3306->MySQL
    switch (port) {
        case 80:   return "HTTP";
        case 443:  return "HTTPS";
        case 22:   return "SSH";
        case 21:   return "FTP";
        case 53:   return "DNS";
        case 25:   return "SMTP";
        case 3306: return "MySQL";
        default:   return "";
    }
}

// ============================================================================
// PROTOCOL LOOKUP
// ============================================================================

// Lookup protocol names
const char* get_protocol_name(unsigned char protocol) {
    switch (protocol) {
        case 1:  return "ICMP";
        case 6:  return "TCP";
        case 17: return "UDP";
        default: return "Unknown";
    }
}

// ============================================================================
// MAIN PARSING FUNCTIONS
// ============================================================================

// Parse and display IPv4 header
void parse_ipv4_header(FILE *file, const char *filename) {
    // TODO: Implement IPv4 header parsing
    // Steps:
    // 1. Read IPv4Header struct from file using fread()
    // 2. Check if read was successful (fread returns 1 for success)
    // 3. Extract version and validate it's IPv4 (version == 4)
    // 4. Extract all other header fields using helper functions
    // 5. Convert IP addresses and ports from network byte order
    // 6. Print all fields in formatted output

    printf("=== Packet Header Parser ===\n");
    printf("File: %s\n", filename);

    // Placeholder - to be implemented
    printf("TODO: Implement IPv4 header parsing\n");
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main(int argc, char *argv[]) {
    // TODO: Add command-line argument validation
    // Check that exactly one argument (filename) is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <packet_file.bin>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    // TODO: Open the file for reading in binary mode
    // Use "rb" mode
    FILE *file = NULL;
    // TODO: Add error checking - if fopen fails, print error and return 1

    // TODO: Get file size for validation
    // Hint: Use fseek, ftell, and fseek again to reset position

    // TODO: Validate file size is at least large enough for IPv4 header (20 bytes)
    // If not, print error and return 1

    // TODO: Call parse_ipv4_header to parse and display IP header
    parse_ipv4_header(file, filename);

    // TODO: Close the file

    return 0;
}

// ============================================================================
// HELPFUL NOTES
// ============================================================================
/*
 * KEY POINTS FOR THIS PROJECT:
 *
 * 1. BYTE ORDER CONVERSION
 *    - Network data uses big-endian (network byte order)
 *    - Your computer uses host byte order (usually little-endian on x86)
 *    - Use ntohs() for 16-bit, ntohl() for 32-bit values
 *    - Example: unsigned short port = ntohs(header.source_port);
 *
 * 2. BIT EXTRACTION
 *    - Right shift to move bits to position 0: (byte >> bit_pos)
 *    - AND with mask to isolate bits: & 0xF for 4 bits, & 0x3F for 6 bits
 *    - Combined: unsigned char version = (version_ihl >> 4) & 0xF;
 *
 * 3. STRUCT PACKING
 *    - Always use #pragma pack(push, 1) before struct definition
 *    - And #pragma pack(pop) after to remove padding
 *    - Network packets have no padding - all fields are adjacent
 *
 * 4. FILE OPERATIONS
 *    - Use fopen(filename, "rb") for binary reading
 *    - Use fread(&struct, sizeof(struct), 1, file) to read struct
 *    - Check return value: fread returns 1 if successful
 *    - Always fclose(file) when done
 *
 * 5. ERROR CHECKING
 *    - Check fopen() returns non-NULL
 *    - Check fread() returns 1 (successful read)
 *    - Check file size before reading headers
 *    - Validate version is 4 (IPv4)
 *
 * 6. COMMON FIELDS TO EXTRACT
 *    - Version: top 4 bits of first byte -> (version_ihl >> 4) & 0xF
 *    - IHL: bottom 4 bits of first byte -> version_ihl & 0xF
 *    - DSCP: top 6 bits of second byte -> (dscp_ecn >> 2) & 0x3F
 *    - ECN: bottom 2 bits of second byte -> dscp_ecn & 0x3
 *    - TCP Flags: individual bits of flags byte
 */
