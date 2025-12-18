#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

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

// ============================================================================
// BIT EXTRACTION HELPER FUNCTIONS
// ============================================================================

unsigned char get_ip_version(unsigned char version_ihl) {
    return (version_ihl >> 4) & 0xF;
}

unsigned char get_ihl(unsigned char version_ihl) {
    return version_ihl & 0xF;
}

unsigned char get_dscp(unsigned char dscp_ecn) {
    return (dscp_ecn >> 2) & 0x3F;
}

unsigned char get_ecn(unsigned char dscp_ecn) {
    return dscp_ecn & 0x3;
}

void get_ip_flags(unsigned short flags_offset, int *reserved, int *dont_fragment, int *more_fragments) {
    unsigned short net_flags = ntohs(flags_offset);
    *reserved = (net_flags >> 15) & 1;
    *dont_fragment = (net_flags >> 14) & 1;
    *more_fragments = (net_flags >> 13) & 1;
}

unsigned short get_fragment_offset(unsigned short flags_offset) {
    unsigned short net_flags = ntohs(flags_offset);
    return net_flags & 0x1FFF;
}

void get_tcp_flags(unsigned char flags,
                   int *fin, int *syn, int *rst,
                   int *psh, int *ack, int *urg) {
    *fin = (flags >> 0) & 1;
    *syn = (flags >> 1) & 1;
    *rst = (flags >> 2) & 1;
    *psh = (flags >> 3) & 1;
    *ack = (flags >> 4) & 1;
    *urg = (flags >> 5) & 1;
}

unsigned char get_tcp_data_offset(unsigned char data_offset) {
    return (data_offset >> 4) & 0xF;
}

// ============================================================================
// IP ADDRESS FORMATTING
// ============================================================================

void format_ip_address(unsigned int ip_bytes, char *buffer, size_t buffer_size) {
    // IP is in network byte order, need to convert back to host order
    unsigned int host_ip = ntohl(ip_bytes);
    unsigned char *bytes = (unsigned char *)&host_ip;
    snprintf(buffer, buffer_size, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}

// ============================================================================
// PORT NUMBER LOOKUP
// ============================================================================

const char* get_port_name(unsigned short port) {
    switch (port) {
        case 20:   return "FTP-DATA";
        case 21:   return "FTP";
        case 22:   return "SSH";
        case 25:   return "SMTP";
        case 53:   return "DNS";
        case 80:   return "HTTP";
        case 443:  return "HTTPS";
        case 3306: return "MySQL";
        case 5432: return "PostgreSQL";
        case 8080: return "HTTP-ALT";
        default:   return "";
    }
}

// ============================================================================
// PROTOCOL LOOKUP
// ============================================================================

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

void parse_ipv4_header(FILE *file, const char *filename) {
    IPv4Header ip_header;

    printf("=== Packet Header Parser ===\n");
    printf("File: %s\n", filename);

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("File size: %ld bytes\n\n", file_size);

    // Read IPv4 header
    if (fread(&ip_header, sizeof(IPv4Header), 1, file) != 1) {
        fprintf(stderr, "Error: Could not read IPv4 header\n");
        return;
    }

    // Parse IPv4 header fields
    unsigned char version = get_ip_version(ip_header.version_ihl);
    unsigned char ihl = get_ihl(ip_header.version_ihl);
    unsigned char dscp = get_dscp(ip_header.dscp_ecn);
    unsigned char ecn = get_ecn(ip_header.dscp_ecn);
    int reserved, dont_fragment, more_fragments;
    get_ip_flags(ip_header.flags_offset, &reserved, &dont_fragment, &more_fragments);
    unsigned short frag_offset = get_fragment_offset(ip_header.flags_offset);

    unsigned short total_len = ntohs(ip_header.total_length);
    unsigned short ident = ntohs(ip_header.identification);
    unsigned short checksum = ntohs(ip_header.header_checksum);

    char src_ip_str[16], dst_ip_str[16];
    format_ip_address(ip_header.source_ip, src_ip_str, sizeof(src_ip_str));
    format_ip_address(ip_header.dest_ip, dst_ip_str, sizeof(dst_ip_str));

    // Display IPv4 header
    printf("--- IP Header (IPv%d) ---\n", version);
    printf("Version: %d\n", version);
    printf("Header Length (IHL): %d words (%d bytes)\n", ihl, ihl * 4);
    printf("DSCP: %d\n", dscp);
    printf("ECN: %d\n", ecn);
    printf("Total Packet Length: %d bytes\n", total_len);
    printf("Identification: 0x%04x\n", ident);
    printf("Reserved: %s\n", reserved ? "Yes" : "No");
    printf("Don't Fragment: %s\n", dont_fragment ? "Yes" : "No");
    printf("More Fragments: %s\n", more_fragments ? "Yes" : "No");
    printf("Fragment Offset: %d\n", frag_offset);
    printf("TTL: %d\n", ip_header.ttl);
    printf("Protocol: %d (%s)\n", ip_header.protocol, get_protocol_name(ip_header.protocol));
    printf("Header Checksum: 0x%04x\n", checksum);
    printf("Source IP: %s\n", src_ip_str);
    printf("Destination IP: %s\n", dst_ip_str);
    printf("\n");

    // Parse TCP or UDP based on protocol
    if (ip_header.protocol == 6) {  // TCP
        TCPHeader tcp_header;
        if (fread(&tcp_header, sizeof(TCPHeader), 1, file) != 1) {
            fprintf(stderr, "Error: Could not read TCP header\n");
            return;
        }

        // Parse TCP flags
        int fin, syn, rst, psh, ack, urg;
        get_tcp_flags(tcp_header.flags, &fin, &syn, &rst, &psh, &ack, &urg);
        unsigned char data_offset = get_tcp_data_offset(tcp_header.data_offset);

        unsigned short src_port = ntohs(tcp_header.source_port);
        unsigned short dst_port = ntohs(tcp_header.dest_port);
        unsigned int seq_num = ntohl(tcp_header.sequence_num);
        unsigned int ack_num = ntohl(tcp_header.ack_num);
        unsigned short window = ntohs(tcp_header.window_size);
        unsigned short tcp_checksum = ntohs(tcp_header.checksum);
        unsigned short urgent = ntohs(tcp_header.urgent_pointer);

        const char *src_port_name = get_port_name(src_port);
        const char *dst_port_name = get_port_name(dst_port);

        printf("--- TCP Header ---\n");
        printf("Source Port: %d", src_port);
        if (strlen(src_port_name) > 0) printf(" (%s)", src_port_name);
        printf("\n");

        printf("Destination Port: %d", dst_port);
        if (strlen(dst_port_name) > 0) printf(" (%s)", dst_port_name);
        printf("\n");

        printf("Sequence Number: 0x%08x\n", seq_num);
        printf("Acknowledgment Number: 0x%08x\n", ack_num);
        printf("Data Offset: %d words (%d bytes)\n", data_offset, data_offset * 4);
        printf("Flags: ");
        if (syn) printf("SYN ");
        if (ack) printf("ACK ");
        if (fin) printf("FIN ");
        if (rst) printf("RST ");
        if (psh) printf("PSH ");
        if (urg) printf("URG ");
        printf("\n");

        printf("  - FIN: %s\n", fin ? "Yes" : "No");
        printf("  - SYN: %s\n", syn ? "Yes" : "No");
        printf("  - RST: %s\n", rst ? "Yes" : "No");
        printf("  - PSH: %s\n", psh ? "Yes" : "No");
        printf("  - ACK: %s\n", ack ? "Yes" : "No");
        printf("  - URG: %s\n", urg ? "Yes" : "No");

        printf("Window Size: %d\n", window);
        printf("Checksum: 0x%04x\n", tcp_checksum);
        printf("Urgent Pointer: %d\n", urgent);

        // Display remaining bytes as payload
        long current_pos = ftell(file);
        long payload_len = file_size - current_pos;
        printf("\n--- Payload ---\n");
        printf("Remaining bytes: %ld\n", payload_len);

    } else if (ip_header.protocol == 17) {  // UDP
        UDPHeader udp_header;
        if (fread(&udp_header, sizeof(UDPHeader), 1, file) != 1) {
            fprintf(stderr, "Error: Could not read UDP header\n");
            return;
        }

        unsigned short src_port = ntohs(udp_header.source_port);
        unsigned short dst_port = ntohs(udp_header.dest_port);
        unsigned short length = ntohs(udp_header.length);
        unsigned short checksum = ntohs(udp_header.checksum);

        const char *src_port_name = get_port_name(src_port);
        const char *dst_port_name = get_port_name(dst_port);

        printf("--- UDP Header ---\n");
        printf("Source Port: %d", src_port);
        if (strlen(src_port_name) > 0) printf(" (%s)", src_port_name);
        printf("\n");

        printf("Destination Port: %d", dst_port);
        if (strlen(dst_port_name) > 0) printf(" (%s)", dst_port_name);
        printf("\n");

        printf("Length: %d bytes\n", length);
        printf("Checksum: 0x%04x\n", checksum);

        long current_pos = ftell(file);
        long payload_len = file_size - current_pos;
        printf("\n--- Payload ---\n");
        printf("Remaining bytes: %ld\n", payload_len);

    } else {
        printf("--- Other Protocol ---\n");
        printf("Protocol %d is not TCP or UDP\n", ip_header.protocol);
        long current_pos = ftell(file);
        long remaining = file_size - current_pos;
        printf("Remaining data: %ld bytes\n", remaining);
    }
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <packet_file.bin>\n", argv[0]);
        fprintf(stderr, "\nExample:\n");
        fprintf(stderr, "  %s sample_packet.bin\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Validate minimum file size
    if (file_size < (long)sizeof(IPv4Header)) {
        fprintf(stderr, "Error: File too small (need at least %zu bytes for IP header, got %ld)\n",
                sizeof(IPv4Header), file_size);
        fclose(file);
        return 1;
    }

    // Parse and display packet headers
    parse_ipv4_header(file, filename);

    fclose(file);
    return 0;
}
