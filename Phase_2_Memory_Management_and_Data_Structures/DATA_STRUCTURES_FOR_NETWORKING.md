# Data Structures for Networking
## Building Your Protocol Toolkit

This guide provides complete C struct definitions for the network protocol stack. These structures are the foundation for all packet manipulation, parsing, and crafting.

---

## Ethernet Frame (RFC 802.3)

The Ethernet frame is the lowest layer of the network stack, responsible for moving data between devices on the same local network.

```
Destination MAC (6 bytes) | Source MAC (6 bytes) | Type/Length (2 bytes) | Payload (46-1500 bytes) | FCS (4 bytes)

                          ^-- This is what we define in struct
```

**C Definition:**

```c
struct ethernet_frame {
    unsigned char dest_mac[6];      // Destination MAC address
    unsigned char src_mac[6];       // Source MAC address
    unsigned short ethertype;       // EtherType/Length field (network byte order)
} __attribute__((packed));

// Size verification
// _Static_assert(sizeof(struct ethernet_frame) == 14, "Ethernet frame must be 14 bytes");
```

**Common EtherType Values:**

```c
#define ETH_TYPE_IPv4 0x0800        // IPv4
#define ETH_TYPE_ARP  0x0806        // ARP
#define ETH_TYPE_IPv6 0x86DD        // IPv6
#define ETH_TYPE_VLAN 0x8100        // VLAN tag

// Usage:
struct ethernet_frame *eth = (struct ethernet_frame *)packet;
unsigned short etype = ntohs(eth->ethertype);

if (etype == ETH_TYPE_IPv4) {
    printf("IPv4 packet\n");
} else if (etype == ETH_TYPE_ARP) {
    printf("ARP packet\n");
}
```

**Parsing Example:**

```c
void parse_ethernet(unsigned char *packet) {
    struct ethernet_frame *eth = (struct ethernet_frame *)packet;

    // Display MAC addresses
    printf("Dest MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->dest_mac[0], eth->dest_mac[1], eth->dest_mac[2],
           eth->dest_mac[3], eth->dest_mac[4], eth->dest_mac[5]);

    printf("Src MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->src_mac[0], eth->src_mac[1], eth->src_mac[2],
           eth->src_mac[3], eth->src_mac[4], eth->src_mac[5]);

    printf("EtherType: 0x%04x\n", ntohs(eth->ethertype));
}
```

---

## IPv4 Header (RFC 791)

IPv4 is the primary network layer protocol. The header is variable-length (20-60 bytes) because of optional fields.

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Version|  IHL  |Type of Service|          Total Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Identification        |Flags|      Fragment Offset    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Time to Live |    Protocol   |         Header Checksum       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                       Source Address                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Destination Address                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Options (if IHL > 5)                       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

**C Definition:**

```c
struct ipv4_header {
    unsigned char version_ihl;         // Version (4 bits) + IHL (4 bits)
    unsigned char dscp_ecn;            // DSCP (6 bits) + ECN (2 bits)
    unsigned short total_length;       // Total packet length (network byte order)
    unsigned short identification;     // Unique ID for fragments (network byte order)
    unsigned short flags_frag_offset;  // Flags (3 bits) + Fragment offset (13 bits, network byte order)
    unsigned char ttl;                 // Time To Live
    unsigned char protocol;            // Protocol number (6=TCP, 17=UDP, 50=IPsec ESP, etc.)
    unsigned short header_checksum;    // Checksum (network byte order)
    struct in_addr src_addr;           // Source IPv4 address (network byte order, 4 bytes)
    struct in_addr dest_addr;          // Destination IPv4 address (network byte order, 4 bytes)
} __attribute__((packed));

// Size verification
// _Static_assert(sizeof(struct ipv4_header) == 20, "IPv4 header base is 20 bytes");
```

**Common Protocol Numbers:**

```c
#define PROTO_ICMP      1
#define PROTO_IGMP      2
#define PROTO_TCP       6
#define PROTO_UDP       17
#define PROTO_IPsec_ESP 50
#define PROTO_IPsec_AH  51
#define PROTO_IPv6      41
```

**Helper Functions:**

```c
// Extract version and IHL from first byte
unsigned char get_version(struct ipv4_header *hdr) {
    return (hdr->version_ihl >> 4) & 0x0F;
}

unsigned char get_ihl(struct ipv4_header *hdr) {
    return hdr->version_ihl & 0x0F;
}

// Calculate header length in bytes (IHL is in 32-bit words)
int get_header_length_bytes(struct ipv4_header *hdr) {
    return get_ihl(hdr) * 4;
}

// Extract flags
unsigned char get_df_flag(struct ipv4_header *hdr) {
    return (ntohs(hdr->flags_frag_offset) >> 14) & 1;
}

unsigned char get_mf_flag(struct ipv4_header *hdr) {
    return (ntohs(hdr->flags_frag_offset) >> 13) & 1;
}

// Extract fragment offset (in 8-byte units)
unsigned short get_fragment_offset(struct ipv4_header *hdr) {
    return ntohs(hdr->flags_frag_offset) & 0x1FFF;
}

// Parsing example
void parse_ipv4(unsigned char *packet) {
    struct ipv4_header *ip = (struct ipv4_header *)(packet + 14);  // After Ethernet

    printf("Version: %d\n", get_version(ip));
    printf("Header Length: %d bytes\n", get_header_length_bytes(ip));
    printf("Total Length: %d bytes\n", ntohs(ip->total_length));
    printf("TTL: %d\n", ip->ttl);
    printf("Protocol: %d\n", ip->protocol);

    // Convert addresses to dotted decimal
    struct in_addr src, dst;
    src.s_addr = ip->src_addr.s_addr;
    dst.s_addr = ip->dest_addr.s_addr;
    printf("Src: %s\n", inet_ntoa(src));
    printf("Dst: %s\n", inet_ntoa(dst));
}
```

---

## TCP Header (RFC 793)

TCP is the connection-oriented transport layer protocol. The header is also variable-length (20-60 bytes) due to options.

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                        Sequence Number                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Acknowledgment Number                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Data |           |U|A|P|R|S|F|                               |
| Offset| Reserved |R|C|S|S|Y|I|            Window             |
|       |           |G|K|H|T|N|N|                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Checksum            |       Urgent Pointer          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Options (if data offset > 5)               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

**C Definition:**

```c
struct tcp_header {
    unsigned short src_port;           // Source port (network byte order)
    unsigned short dest_port;          // Destination port (network byte order)
    unsigned int sequence_number;      // Sequence number (network byte order)
    unsigned int ack_number;           // Acknowledgment number (network byte order)
    unsigned char data_offset_reserved; // Data offset (4 bits) + Reserved (4 bits)
    unsigned char flags;               // URG|ACK|PSH|RST|SYN|FIN (6 bits) + Reserved (2 bits)
    unsigned short window_size;        // Window size (network byte order)
    unsigned short checksum;           // Checksum (network byte order)
    unsigned short urgent_pointer;     // Urgent pointer (network byte order)
} __attribute__((packed));

// Size verification
// _Static_assert(sizeof(struct tcp_header) == 20, "TCP header base is 20 bytes");
```

**Flag Definitions:**

```c
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PSH 0x08
#define TCP_ACK 0x10
#define TCP_URG 0x20

// Helper to extract flags
unsigned char tcp_get_fin(struct tcp_header *hdr) { return (hdr->flags >> 0) & 1; }
unsigned char tcp_get_syn(struct tcp_header *hdr) { return (hdr->flags >> 1) & 1; }
unsigned char tcp_get_rst(struct tcp_header *hdr) { return (hdr->flags >> 2) & 1; }
unsigned char tcp_get_psh(struct tcp_header *hdr) { return (hdr->flags >> 3) & 1; }
unsigned char tcp_get_ack(struct tcp_header *hdr) { return (hdr->flags >> 4) & 1; }
unsigned char tcp_get_urg(struct tcp_header *hdr) { return (hdr->flags >> 5) & 1; }

// Get TCP header length in bytes
int tcp_get_header_length(struct tcp_header *hdr) {
    return ((hdr->data_offset_reserved >> 4) & 0x0F) * 4;
}

// Parsing example
void parse_tcp(unsigned char *packet, int ip_header_len) {
    struct tcp_header *tcp = (struct tcp_header *)(packet + 14 + ip_header_len);

    printf("Src port: %d\n", ntohs(tcp->src_port));
    printf("Dst port: %d\n", ntohs(tcp->dest_port));
    printf("Sequence: %u\n", ntohl(tcp->sequence_number));
    printf("Ack: %u\n", ntohl(tcp->ack_number));
    printf("Flags: ");
    if (tcp_get_fin(tcp)) printf("FIN ");
    if (tcp_get_syn(tcp)) printf("SYN ");
    if (tcp_get_rst(tcp)) printf("RST ");
    if (tcp_get_psh(tcp)) printf("PSH ");
    if (tcp_get_ack(tcp)) printf("ACK ");
    if (tcp_get_urg(tcp)) printf("URG ");
    printf("\n");
    printf("Window: %d\n", ntohs(tcp->window_size));
}
```

---

## UDP Header (RFC 768)

UDP is the simple, connectionless transport layer protocol. The header is fixed at 8 bytes.

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Length            |            Checksum           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

**C Definition:**

```c
struct udp_header {
    unsigned short src_port;           // Source port (network byte order)
    unsigned short dest_port;          // Destination port (network byte order)
    unsigned short length;             // UDP header + payload length (network byte order)
    unsigned short checksum;           // Checksum (network byte order)
} __attribute__((packed));

// Size verification
// _Static_assert(sizeof(struct udp_header) == 8, "UDP header is 8 bytes");

// Parsing example
void parse_udp(unsigned char *packet, int ip_header_len) {
    struct udp_header *udp = (struct udp_header *)(packet + 14 + ip_header_len);

    printf("Src port: %d\n", ntohs(udp->src_port));
    printf("Dst port: %d\n", ntohs(udp->dest_port));
    printf("Length: %d\n", ntohs(udp->length));
    printf("Checksum: 0x%04x\n", ntohs(udp->checksum));
}
```

---

## ICMP Header (RFC 792)

ICMP is used for network diagnostics (ping, traceroute). Headers vary by type.

**Echo Request/Reply (Type 8/0):**

```c
struct icmp_echo {
    unsigned char type;                // Type (8=request, 0=reply)
    unsigned char code;                // Code (always 0 for echo)
    unsigned short checksum;           // Checksum (network byte order)
    unsigned short identifier;         // Identifier (network byte order)
    unsigned short sequence;           // Sequence number (network byte order)
    // Followed by optional data
} __attribute__((packed));

// Parsing example
void parse_icmp_echo(unsigned char *packet, int ip_header_len) {
    struct icmp_echo *icmp = (struct icmp_echo *)(packet + 14 + ip_header_len);

    printf("Type: %d (%s)\n", icmp->type, icmp->type == 8 ? "Request" : "Reply");
    printf("Code: %d\n", icmp->code);
    printf("Identifier: %d\n", ntohs(icmp->identifier));
    printf("Sequence: %d\n", ntohs(icmp->sequence));
}
```

**Destination Unreachable (Type 3):**

```c
struct icmp_unreachable {
    unsigned char type;                // Type 3
    unsigned char code;                // 0=net, 1=host, 2=protocol, 3=port, etc.
    unsigned short checksum;           // Checksum
    unsigned int unused;               // Unused (all zeros)
    // Followed by 8 bytes of original IP header + 8 bytes of data
} __attribute__((packed));
```

---

## ARP Header (RFC 826)

ARP resolves IPv4 addresses to MAC addresses on a local network.

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Hardware Type         |         Protocol Type         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Hw Len  |  Prot Len  |           Operation                   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Sender Hardware Address                    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Sender Protocol Address                    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Target Hardware Address                    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Target Protocol Address                    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

**C Definition (for IPv4 over Ethernet):**

```c
struct arp_header {
    unsigned short hardware_type;      // 1 = Ethernet (network byte order)
    unsigned short protocol_type;      // 0x0800 = IPv4 (network byte order)
    unsigned char hw_addr_len;         // 6 for Ethernet
    unsigned char proto_addr_len;      // 4 for IPv4
    unsigned short operation;          // 1=Request, 2=Reply (network byte order)
    unsigned char sender_hw[6];        // Sender MAC address
    struct in_addr sender_proto;       // Sender IPv4 address (network byte order)
    unsigned char target_hw[6];        // Target MAC address
    struct in_addr target_proto;       // Target IPv4 address (network byte order)
} __attribute__((packed));

// Size verification
// _Static_assert(sizeof(struct arp_header) == 28, "ARP header for IPv4/Ethernet is 28 bytes");

#define ARP_OP_REQUEST 1
#define ARP_OP_REPLY   2

// Parsing example
void parse_arp(unsigned char *packet) {
    struct arp_header *arp = (struct arp_header *)(packet + 14);

    printf("Hardware type: %d\n", ntohs(arp->hardware_type));
    printf("Protocol type: 0x%04x\n", ntohs(arp->protocol_type));
    printf("Operation: %s\n", ntohs(arp->operation) == ARP_OP_REQUEST ? "Request" : "Reply");

    printf("Sender MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp->sender_hw[0], arp->sender_hw[1], arp->sender_hw[2],
           arp->sender_hw[3], arp->sender_hw[4], arp->sender_hw[5]);
    printf("Sender IP: %s\n", inet_ntoa(arp->sender_proto));

    printf("Target MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp->target_hw[0], arp->target_hw[1], arp->target_hw[2],
           arp->target_hw[3], arp->target_hw[4], arp->target_hw[5]);
    printf("Target IP: %s\n", inet_ntoa(arp->target_proto));
}
```

---

## IPsec ESP Header (RFC 4303)

For your packet loss detection use case, the IPsec ESP header is critical.

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|               Security Parameters Index (SPI)                 |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                      Sequence Number                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                 Payload Data* (variable)                      |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                 Padding (0-255 bytes)                         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|              Pad Length       |  Next Header                  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Authentication Data (variable)                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

**C Definition:**

```c
struct ipsec_esp_header {
    unsigned int spi;                  // Security Parameters Index (network byte order)
    unsigned int sequence_number;      // **THE CRITICAL FIELD FOR LOSS DETECTION** (network byte order)
} __attribute__((packed));

// Size verification
// _Static_assert(sizeof(struct ipsec_esp_header) == 8, "IPsec ESP header base is 8 bytes");

// Your packet loss detection function
void analyze_ipsec_packet(unsigned char *packet, int packet_len) {
    // Skip Ethernet (14 bytes)
    unsigned char *ptr = packet + 14;

    // Get IP header
    struct ipv4_header *ip = (struct ipv4_header *)ptr;
    int ip_header_len = (ip->version_ihl & 0x0F) * 4;

    // Verify it's IPsec ESP
    if (ip->protocol != 50) {  // 50 = IPsec ESP
        printf("Not an IPsec ESP packet\n");
        return;
    }

    // Get ESP header
    struct ipsec_esp_header *esp = (struct ipsec_esp_header *)(ptr + ip_header_len);

    unsigned int spi = ntohl(esp->spi);
    unsigned int seq = ntohl(esp->sequence_number);

    printf("IPsec ESP - SPI: 0x%x, Sequence: %u\n", spi, seq);

    // Track for loss detection
    static unsigned int last_spi = 0;
    static unsigned int last_seq = 0;

    if (spi == last_spi) {
        if (seq != last_seq + 1) {
            unsigned int lost = seq - last_seq - 1;
            printf("PACKET LOSS: %u packets lost between seq %u and %u\n", lost, last_seq, seq);
        }
    }

    last_spi = spi;
    last_seq = seq;
}
```

---

## MACsec Header (IEEE 802.1AE)

MACsec provides frame-level encryption and authentication.

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      Version  | AS|C|E|SC |           Packet Number           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                   Optional SCI (if SC bit set)                |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          Encrypted Payload                    |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

**C Definition (Simplified):**

```c
struct macsec_header {
    unsigned char version_tci;         // Version (2 bits) + TCI (6 bits)
    // TCI bits: AS|C|E|SC (4 bits) + reserved (2 bits)
    unsigned char reserved_sc_flags;   // Reserved + SC flag
    unsigned short packet_number;      // **PACKET LOSS TRACKING** (network byte order)
} __attribute__((packed));

// Helper functions
unsigned char macsec_get_version(struct macsec_header *hdr) {
    return (hdr->version_tci >> 6) & 0x03;
}

unsigned char macsec_get_as_flag(struct macsec_header *hdr) {
    return (hdr->version_tci >> 5) & 1;
}

unsigned char macsec_get_c_flag(struct macsec_header *hdr) {
    return (hdr->version_tci >> 4) & 1;
}

unsigned char macsec_get_e_flag(struct macsec_header *hdr) {
    return (hdr->version_tci >> 3) & 1;
}

unsigned char macsec_get_sc_flag(struct macsec_header *hdr) {
    return (hdr->version_tci >> 2) & 1;
}

// Parsing example
void analyze_macsec_packet(unsigned char *packet, int packet_len) {
    struct ethernet_frame *eth = (struct ethernet_frame *)packet;

    // Check if this is a MACsec frame (EtherType 0x88E5)
    if (ntohs(eth->ethertype) != 0x88E5) {
        printf("Not a MACsec packet\n");
        return;
    }

    struct macsec_header *macsec = (struct macsec_header *)(packet + 14);

    unsigned short pkt_num = ntohs(macsec->packet_number);
    printf("MACsec - Packet Number: %u\n", pkt_num);
    printf("Version: %d\n", macsec_get_version(macsec));
    printf("AS flag: %d\n", macsec_get_as_flag(macsec));
    printf("C flag: %d\n", macsec_get_c_flag(macsec));
    printf("E flag: %d\n", macsec_get_e_flag(macsec));
    printf("SC flag: %d\n", macsec_get_sc_flag(macsec));

    // Track for loss detection
    static unsigned short last_pkt_num = 0;
    if (pkt_num != last_pkt_num + 1) {
        unsigned int lost = pkt_num - last_pkt_num - 1;
        printf("PACKET LOSS: %u packets lost between #%u and #%u\n", lost, last_pkt_num, pkt_num);
    }
    last_pkt_num = pkt_num;
}
```

---

## Complete Packet Parsing Example

Here's how all these structures work together:

```c
void parse_complete_packet(unsigned char *packet, int packet_len) {
    int offset = 0;

    // Layer 2: Ethernet
    if (offset + sizeof(struct ethernet_frame) > packet_len) {
        printf("Packet too short for Ethernet\n");
        return;
    }

    struct ethernet_frame *eth = (struct ethernet_frame *)(packet + offset);
    unsigned short ethertype = ntohs(eth->ethertype);
    printf("=== Ethernet Frame ===\n");
    printf("Src MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->src_mac[0], eth->src_mac[1], eth->src_mac[2],
           eth->src_mac[3], eth->src_mac[4], eth->src_mac[5]);
    printf("EtherType: 0x%04x\n", ethertype);
    offset += sizeof(struct ethernet_frame);

    // Layer 3: IPv4
    if (ethertype != 0x0800) {
        printf("Not IPv4\n");
        return;
    }

    if (offset + sizeof(struct ipv4_header) > packet_len) {
        printf("Packet too short for IPv4\n");
        return;
    }

    struct ipv4_header *ip = (struct ipv4_header *)(packet + offset);
    int ip_header_len = (ip->version_ihl & 0x0F) * 4;
    printf("\n=== IPv4 Header ===\n");
    printf("Src: %s\n", inet_ntoa(ip->src_addr));
    printf("Dst: %s\n", inet_ntoa(ip->dest_addr));
    printf("Protocol: %d\n", ip->protocol);
    printf("TTL: %d\n", ip->ttl);
    offset += ip_header_len;

    // Layer 4: Depends on protocol
    if (ip->protocol == 6) {  // TCP
        if (offset + sizeof(struct tcp_header) > packet_len) {
            printf("Packet too short for TCP\n");
            return;
        }

        struct tcp_header *tcp = (struct tcp_header *)(packet + offset);
        printf("\n=== TCP Header ===\n");
        printf("Src port: %d\n", ntohs(tcp->src_port));
        printf("Dst port: %d\n", ntohs(tcp->dest_port));
        printf("Flags: ");
        if (tcp->flags & TCP_SYN) printf("SYN ");
        if (tcp->flags & TCP_ACK) printf("ACK ");
        if (tcp->flags & TCP_FIN) printf("FIN ");
        printf("\n");

    } else if (ip->protocol == 17) {  // UDP
        if (offset + sizeof(struct udp_header) > packet_len) {
            printf("Packet too short for UDP\n");
            return;
        }

        struct udp_header *udp = (struct udp_header *)(packet + offset);
        printf("\n=== UDP Header ===\n");
        printf("Src port: %d\n", ntohs(udp->src_port));
        printf("Dst port: %d\n", ntohs(udp->dest_port));

    } else if (ip->protocol == 50) {  // IPsec ESP
        if (offset + sizeof(struct ipsec_esp_header) > packet_len) {
            printf("Packet too short for IPsec ESP\n");
            return;
        }

        struct ipsec_esp_header *esp = (struct ipsec_esp_header *)(packet + offset);
        printf("\n=== IPsec ESP Header ===\n");
        printf("SPI: 0x%x\n", ntohl(esp->spi));
        printf("Sequence: %u\n", ntohl(esp->sequence_number));
    }
}
```

---

## Best Practices for Network Structures

1. **Always use `__attribute__((packed))`** for network protocol structures
2. **Always use `ntohs()`, `ntohl()`, `htons()`, `htonl()`** for multi-byte fields
3. **Verify struct sizes** with `_Static_assert()` or `sizeof()`
4. **Check packet length before casting** to avoid out-of-bounds access
5. **Use helper functions** for bit field extraction
6. **Document field purposes** especially for your use cases (IPsec SPI, MACsec packet number)
7. **Test with real packets** from network captures to verify parsing

---

## Summary

These structures are the foundation of your packet analysis toolkit:
- **Ethernet**: Moves packets between devices on local network
- **IPv4**: Moves packets between hosts across internet
- **TCP/UDP**: Application-level communication protocols
- **ICMP**: Network diagnostics
- **ARP**: Resolves IP addresses to MAC addresses
- **IPsec ESP**: Your primary focus - tracks sequence numbers for loss detection
- **MACsec**: Tracks packet numbers for loss detection

Master these structures and you can parse any packet that comes across your network interface. In Phase 3, you'll actually receive these packets using raw sockets.
