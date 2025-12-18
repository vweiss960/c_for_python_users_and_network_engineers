#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

/*
 * This utility generates sample binary packet files for testing the parser.
 * It demonstrates how to construct valid IP and TCP headers programmatically.
 */

#pragma pack(push, 1)

typedef struct {
    unsigned char version_ihl;
    unsigned char dscp_ecn;
    unsigned short total_length;
    unsigned short identification;
    unsigned short flags_offset;
    unsigned char ttl;
    unsigned char protocol;
    unsigned short header_checksum;
    unsigned int source_ip;
    unsigned int dest_ip;
} IPv4Header;

typedef struct {
    unsigned short source_port;
    unsigned short dest_port;
    unsigned int sequence_num;
    unsigned int ack_num;
    unsigned char data_offset;
    unsigned char flags;
    unsigned short window_size;
    unsigned short checksum;
    unsigned short urgent_pointer;
} TCPHeader;

typedef struct {
    unsigned short source_port;
    unsigned short dest_port;
    unsigned short length;
    unsigned short checksum;
} UDPHeader;

#pragma pack(pop)

// Calculate simple IP header checksum (not cryptographically valid)
unsigned short calculate_checksum(unsigned short *data, int len) {
    unsigned int sum = 0;
    for (int i = 0; i < len / 2; i++) {
        sum += data[i];
    }
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return ~sum;
}

// Create an IPv4 + TCP packet
void create_ipv4_tcp_packet(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    // Create IPv4 header
    IPv4Header ip = {
        .version_ihl = (4 << 4) | 5,        // Version 4, IHL 5 (20 bytes)
        .dscp_ecn = 0,                      // No DSCP, no ECN
        .total_length = htons(60),          // 20 (IP) + 20 (TCP) + 20 (payload)
        .identification = htons(0x1234),    // Identification
        .flags_offset = htons(0x4000),      // Don't fragment flag set, no fragment offset
        .ttl = 64,                          // Time to live
        .protocol = 6,                      // Protocol 6 = TCP
        .header_checksum = 0,               // Will calculate
        .source_ip = inet_addr("192.168.1.100"),   // Source IP
        .dest_ip = inet_addr("10.0.0.50")          // Destination IP
    };

    // Calculate IP header checksum
    ip.header_checksum = calculate_checksum((unsigned short *)&ip, sizeof(IPv4Header));

    // Create TCP header
    TCPHeader tcp = {
        .source_port = htons(54321),        // Source port
        .dest_port = htons(80),             // Destination port (HTTP)
        .sequence_num = htonl(0x12345678),  // Sequence number
        .ack_num = htonl(0x87654321),       // Acknowledgment number
        .data_offset = (5 << 4),            // Data offset 5 words (20 bytes)
        .flags = 0x02 | 0x10,               // SYN (0x02) | ACK (0x10) = 0x12
        .window_size = htons(32768),        // Window size
        .checksum = 0,                      // Would need pseudo-header to calculate
        .urgent_pointer = 0                 // No urgent data
    };

    // Write headers to file
    fwrite(&ip, sizeof(IPv4Header), 1, file);
    fwrite(&tcp, sizeof(TCPHeader), 1, file);

    // Write some payload (20 bytes)
    unsigned char payload[20] = {
        'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l',
        'd', '!', '\n', 0, 0, 0, 0, 0, 0, 0
    };
    fwrite(payload, 20, 1, file);

    fclose(file);
    printf("Created %s (IPv4 + TCP packet, 60 bytes)\n", filename);
}

// Create an IPv4 + UDP packet
void create_ipv4_udp_packet(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    // Create IPv4 header
    IPv4Header ip = {
        .version_ihl = (4 << 4) | 5,
        .dscp_ecn = 0,
        .total_length = htons(40),          // 20 (IP) + 8 (UDP) + 12 (payload)
        .identification = htons(0x5678),
        .flags_offset = htons(0x4000),      // Don't fragment
        .ttl = 64,
        .protocol = 17,                     // Protocol 17 = UDP
        .header_checksum = 0,
        .source_ip = inet_addr("192.168.1.50"),
        .dest_ip = inet_addr("8.8.8.8")
    };

    // Calculate checksum
    ip.header_checksum = calculate_checksum((unsigned short *)&ip, sizeof(IPv4Header));

    // Create UDP header
    UDPHeader udp = {
        .source_port = htons(53),           // DNS source
        .dest_port = htons(53),             // DNS destination
        .length = htons(20),                // 8 (UDP) + 12 (payload)
        .checksum = 0                       // No checksum (would need pseudo-header)
    };

    // Write headers
    fwrite(&ip, sizeof(IPv4Header), 1, file);
    fwrite(&udp, sizeof(UDPHeader), 1, file);

    // Write payload (12 bytes)
    unsigned char payload[12] = {
        'D', 'N', 'S', ' ', 'Q', 'u', 'e', 'r', 'y', 0, 0, 0
    };
    fwrite(payload, 12, 1, file);

    fclose(file);
    printf("Created %s (IPv4 + UDP packet, 40 bytes)\n", filename);
}

// Create a simple test packet with minimal data
void create_minimal_ipv4_packet(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    IPv4Header ip = {
        .version_ihl = (4 << 4) | 5,
        .dscp_ecn = 0,
        .total_length = htons(20),          // Just the IP header, no payload
        .identification = htons(0x0001),
        .flags_offset = 0,
        .ttl = 255,
        .protocol = 0,                      // Unspecified protocol
        .header_checksum = 0,
        .source_ip = inet_addr("127.0.0.1"),
        .dest_ip = inet_addr("127.0.0.1")
    };

    ip.header_checksum = calculate_checksum((unsigned short *)&ip, sizeof(IPv4Header));

    fwrite(&ip, sizeof(IPv4Header), 1, file);

    fclose(file);
    printf("Created %s (minimal IPv4 packet, 20 bytes)\n", filename);
}

int main(int argc, char *argv[]) {
    printf("Packet Generator - Creates sample binary packet files\n\n");

    // Generate sample packets
    create_ipv4_tcp_packet("sample_packet.bin");
    create_ipv4_udp_packet("sample_udp_packet.bin");
    create_minimal_ipv4_packet("minimal_packet.bin");

    printf("\nGenerated packets:\n");
    printf("  sample_packet.bin - IPv4 + TCP packet (60 bytes)\n");
    printf("  sample_udp_packet.bin - IPv4 + UDP packet (40 bytes)\n");
    printf("  minimal_packet.bin - IPv4 only packet (20 bytes)\n");
    printf("\nTest with:\n");
    printf("  ./parser sample_packet.bin\n");
    printf("  ./parser sample_udp_packet.bin\n");
    printf("  ./parser minimal_packet.bin\n");

    return 0;
}
