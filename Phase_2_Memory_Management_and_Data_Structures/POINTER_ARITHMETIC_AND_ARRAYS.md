# Pointer Arithmetic and Arrays
## Navigating Binary Data

Pointer arithmetic is how you navigate through packet buffers, parse binary data, and access array elements. Master this and packet parsing becomes straightforward.

---

## Arrays and Pointers - The Connection

In C, arrays and pointers are closely related:

```c
int arr[5] = {10, 20, 30, 40, 50};
int *ptr = arr;  // ptr points to first element

// These are equivalent:
printf("%d\n", arr[0]);   // 10
printf("%d\n", *ptr);     // 10
printf("%d\n", *(ptr + 0)); // 10

printf("%d\n", arr[2]);   // 30
printf("%d\n", *(ptr + 2)); // 30
```

**Key insight:** `arr[i]` is syntactic sugar for `*(arr + i)`

The compiler automatically converts array syntax to pointer arithmetic.

---

## Understanding Pointer Arithmetic

### The Golden Rule

When you increment a pointer, it moves forward by the **size of the type it points to**, not by 1 byte.

```c
int *iptr = (int *)0x1000;     // Points to address 0x1000
iptr++;                         // Now points to 0x1004 (not 0x1001!)
//                            // Moved forward 4 bytes (size of int)

char *cptr = (char *)0x1000;    // Points to address 0x1000
cptr++;                         // Now points to 0x1001
//                            // Moved forward 1 byte (size of char)

struct packet *pptr = (struct packet *)0x1000;
pptr++;                         // Moves forward sizeof(struct packet) bytes
```

### Arithmetic Operations

**Increment and Decrement:**
```c
int arr[5] = {1, 2, 3, 4, 5};
int *p = arr;

p++;            // Move to next element (arr[1])
printf("%d\n", *p);  // 2

p--;            // Move back (arr[0])
printf("%d\n", *p);  // 1

p += 3;         // Move forward 3 elements (arr[3])
printf("%d\n", *p);  // 4

p -= 2;         // Move back 2 elements (arr[1])
printf("%d\n", *p);  // 2
```

**Addition with Arrays:**
```c
int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int *p = arr;

printf("%d\n", *(p + 0));   // 0
printf("%d\n", *(p + 1));   // 1
printf("%d\n", *(p + 5));   // 5
printf("%d\n", *(p + 9));   // 9

// Pointer subtraction
int *p1 = &arr[5];
int *p2 = &arr[2];
int diff = p1 - p2;         // 3 (elements between them)
```

---

## Practical Examples: Parsing Binary Data

### Example 1: Ethernet Frame Parsing

```c
// Raw packet data from network
unsigned char raw_packet[1500];  // Received from network

// Parse by incrementing pointer
unsigned char *ptr = raw_packet;

// Ethernet header is 14 bytes
struct ethernet_frame {
    unsigned char dest_mac[6];
    unsigned char src_mac[6];
    unsigned short ethertype;  // Big-endian
} __attribute__((packed));

struct ethernet_frame *eth = (struct ethernet_frame *)ptr;
printf("Dest MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
       eth->dest_mac[0], eth->dest_mac[1], eth->dest_mac[2],
       eth->dest_mac[3], eth->dest_mac[4], eth->dest_mac[5]);

// Move to next header
ptr += sizeof(struct ethernet_frame);  // ptr now at byte 14

// Check what's next based on EtherType
unsigned short ethertype = ntohs(eth->ethertype);
if (ethertype == 0x0800) {  // IPv4
    struct ipv4_header *ip = (struct ipv4_header *)ptr;
    printf("IP src: %d.%d.%d.%d\n",
           ip->src_addr.s_addr & 0xFF,
           (ip->src_addr.s_addr >> 8) & 0xFF,
           (ip->src_addr.s_addr >> 16) & 0xFF,
           (ip->src_addr.s_addr >> 24) & 0xFF);

    // Move to payload based on IP header length
    int ip_header_len = (ip->version_ihl & 0x0F) * 4;
    ptr += ip_header_len;

    // Now ptr points to TCP/UDP/ICMP payload
}
```

### Example 2: IPsec ESP Header Parsing

Your specific use case: tracking sequence numbers for packet loss detection.

```c
struct ipsec_esp_header {
    unsigned int spi;              // Security Parameter Index
    unsigned int sequence_number;  // THE critical field you're tracking!
} __attribute__((packed));

void analyze_ipsec_packets(unsigned char *packet_buffer, int packet_len) {
    // Skip Ethernet header (14 bytes)
    unsigned char *ptr = packet_buffer + 14;

    // Skip IP header (usually 20 bytes, but variable with options)
    struct ipv4_header *ip = (struct ipv4_header *)ptr;
    int ip_header_len = (ip->version_ihl & 0x0F) * 4;
    ptr += ip_header_len;

    // Now at IPsec ESP header
    struct ipsec_esp_header *esp = (struct ipsec_esp_header *)ptr;

    unsigned int spi = ntohl(esp->spi);
    unsigned int seq = ntohl(esp->sequence_number);

    printf("IPsec ESP - SPI: 0x%x, Sequence: %u\n", spi, seq);

    // Check for packet loss by tracking sequence numbers
    static unsigned int last_spi = 0;
    static unsigned int last_seq = 0;

    if (spi == last_spi) {
        if (seq != last_seq + 1) {
            unsigned int lost = seq - last_seq - 1;
            printf("PACKET LOSS DETECTED: %u packets lost (seq %u -> %u)\n",
                   lost, last_seq, seq);
        }
    }

    last_spi = spi;
    last_seq = seq;
}
```

### Example 3: Stepping Through Packet Byte by Byte

```c
// Display raw packet hex dump
void print_packet_hex(unsigned char *packet, int len) {
    unsigned char *ptr = packet;

    for (int i = 0; i < len; i++) {
        printf("%02x ", *ptr);
        if ((i + 1) % 16 == 0) printf("\n");
        ptr++;  // Move to next byte
    }
    printf("\n");
}

// Extract multi-byte values at specific offsets
unsigned int get_uint32_at_offset(unsigned char *packet, int offset) {
    unsigned char *ptr = packet + offset;
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}

// Usage
unsigned char packet[1500];
// ... receive packet ...

unsigned int esp_spi = get_uint32_at_offset(packet, 34);  // ESP SPI typically at byte 34
printf("ESP SPI: 0x%x\n", esp_spi);
```

---

## Array Notation vs Pointer Notation

Everything you can do with arrays, you can do with pointers:

```c
int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int *ptr = arr;

// These are equivalent:
arr[5]           *(arr + 5)         *(ptr + 5)         ptr[5]

// All access the same element (value: 5)
```

**For network packets, pointer notation is often clearer:**

```c
// Array notation (less clear for packets)
unsigned char packet[1500];
unsigned char byte_at_14 = packet[14];
unsigned char byte_at_15 = packet[15];

// Pointer notation (clearer intent - advancing through packet)
unsigned char *ptr = packet;
ptr += 14;  // Move to byte 14
unsigned char eth_type_high = *ptr;
ptr++;
unsigned char eth_type_low = *ptr;
```

---

## Common Patterns in Packet Parsing

### Pattern 1: Struct Casting

Cast raw bytes to a known structure:

```c
void parse_with_struct(unsigned char *packet) {
    struct ethernet_frame *eth = (struct ethernet_frame *)packet;
    struct ipv4_header *ip = (struct ipv4_header *)(packet + 14);

    // Use struct member access
    printf("EtherType: 0x%04x\n", ntohs(eth->ethertype));
    printf("Protocol: %d\n", ip->protocol);
}
```

### Pattern 2: Pointer Incrementing with Type Casting

```c
void parse_with_advancement(unsigned char *packet) {
    unsigned char *ptr = packet;

    // Cast current position and use struct
    struct ethernet_frame *eth = (struct ethernet_frame *)ptr;
    printf("EtherType: 0x%04x\n", ntohs(eth->ethertype));

    // Advance past Ethernet header
    ptr += sizeof(struct ethernet_frame);

    // Cast new position
    struct ipv4_header *ip = (struct ipv4_header *)ptr;
    printf("Protocol: %d\n", ip->protocol);

    // Advance past IP header (variable length!)
    int ip_header_len = (ip->version_ihl & 0x0F) * 4;
    ptr += ip_header_len;

    // Now ptr points to transport layer (TCP/UDP/ICMP/ESP)
}
```

### Pattern 3: Manual Byte-by-Byte Access

When you need precise control:

```c
void parse_manually(unsigned char *packet) {
    unsigned char *ptr = packet;

    // Bytes 0-5: Destination MAC
    printf("Dest MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
    ptr += 6;

    // Bytes 6-11: Source MAC
    printf("Src MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
    ptr += 6;

    // Bytes 12-13: EtherType (big-endian)
    unsigned short ethertype = (ptr[0] << 8) | ptr[1];
    printf("EtherType: 0x%04x\n", ethertype);
    ptr += 2;

    // ptr now at byte 14 (start of payload)
}
```

### Pattern 4: Calculating Offsets

For protocols with variable-length headers:

```c
void handle_variable_ip_header(unsigned char *packet) {
    struct ipv4_header *ip = (struct ipv4_header *)(packet + 14);

    // IP header length is variable (IHL field * 4 bytes)
    unsigned char ihl = ip->version_ihl & 0x0F;
    int ip_header_bytes = ihl * 4;

    printf("IP Header: %d bytes\n", ip_header_bytes);

    // Calculate where transport layer starts
    unsigned char *transport_layer = packet + 14 + ip_header_bytes;

    // Now parse based on protocol field
    if (ip->protocol == 6) {  // TCP
        struct tcp_header *tcp = (struct tcp_header *)transport_layer;
        printf("TCP src port: %d\n", ntohs(tcp->src_port));
    } else if (ip->protocol == 17) {  // UDP
        struct udp_header *udp = (struct udp_header *)transport_layer;
        printf("UDP src port: %d\n", ntohs(udp->src_port));
    } else if (ip->protocol == 50) {  // IPsec ESP
        struct ipsec_esp_header *esp = (struct ipsec_esp_header *)transport_layer;
        printf("IPsec SPI: 0x%x\n", ntohl(esp->spi));
        printf("IPsec Sequence: %u\n", ntohl(esp->sequence_number));
    }
}
```

---

## Pointer Arithmetic Pitfalls

### Pitfall 1: Forgetting Type Size

```c
// WRONG: Moves forward 1 byte (not 1 struct!)
struct ethernet_frame *eth = (struct ethernet_frame *)packet;
eth++;  // Only moves forward 1 byte!

// CORRECT:
unsigned char *ptr = packet;
ptr += sizeof(struct ethernet_frame);  // Moves forward 14 bytes
```

### Pitfall 2: Casting Inconsistency

```c
// CONFUSING: Casting to different types
unsigned char *ptr = packet;
int *iptr = (int *)ptr;
iptr++;  // Moves forward 4 bytes
*iptr;   // Accesses bytes 4-7

// BETTER: Be consistent with types
unsigned char *ptr = packet;
ptr += 4;
// Now manually read 4 bytes or cast specifically
unsigned int value = *(unsigned int *)ptr;
```

### Pitfall 3: Out-of-Bounds Access

```c
// DANGER: No bounds checking
unsigned char *ptr = packet;
ptr += 1500;  // Move past end
*ptr = 5;     // Crash! Out of bounds

// SAFE:
unsigned char *ptr = packet;
int offset = 1500;
if (offset < packet_len) {
    ptr += offset;
    *ptr = 5;
}
```

### Pitfall 4: Network Byte Order Confusion

```c
// These are NOT equivalent!
struct ethernet_frame *eth = (struct ethernet_frame *)packet;

// Raw value (network byte order - big-endian)
unsigned short raw = eth->ethertype;  // 0x0800 in memory might be stored as 0x08,0x00

// Converted to host byte order
unsigned short host_order = ntohs(eth->ethertype);  // Now safe to use

printf("Raw: 0x%04x\n", raw);        // Might print incorrectly
printf("Host order: 0x%04x\n", host_order);  // Always correct
```

---

## Advanced: Pointer Arithmetic for Data Structure Navigation

### Dynamic Structure with Pointers

```c
// Imagine: Ethernet -> IPv4 -> TCP -> Application data
// Each layer has variable header size

typedef struct {
    unsigned char *data;
    int length;
    int offset;  // Current read position
} packet_parser;

packet_parser *pp_create(unsigned char *packet, int len) {
    packet_parser *pp = malloc(sizeof(packet_parser));
    pp->data = packet;
    pp->length = len;
    pp->offset = 0;
    return pp;
}

// Read N bytes and advance offset
unsigned char *pp_read(packet_parser *pp, int bytes_to_read) {
    if (pp->offset + bytes_to_read > pp->length) {
        return NULL;  // Would read past end
    }

    unsigned char *result = pp->data + pp->offset;
    pp->offset += bytes_to_read;
    return result;
}

// Usage
packet_parser *pp = pp_create(packet, packet_len);

// Read Ethernet header
struct ethernet_frame *eth = (struct ethernet_frame *)pp_read(pp, 14);

// Read IP header
struct ipv4_header *ip = (struct ipv4_header *)pp_read(pp, 20);

// Read TCP header
struct tcp_header *tcp = (struct tcp_header *)pp_read(pp, 20);

// Remaining is payload
unsigned char *payload = pp_read(pp, pp->length - pp->offset);

pp_free(pp);
```

---

## Practice Exercises

### Exercise 1: Byte-by-Byte Packet Analysis

Write a function that takes a packet and prints:
- Bytes 0-5: Destination MAC
- Bytes 6-11: Source MAC
- Bytes 12-13: EtherType
- First 4 bytes of payload

Use only pointer arithmetic, no struct casting.

### Exercise 2: Parse Ethernet + IPv4 + ICMP

Write a function that:
1. Parses Ethernet header (14 bytes)
2. Checks EtherType is IPv4 (0x0800)
3. Parses IPv4 header (variable length, get size from IHL)
4. Checks protocol is ICMP (1)
5. Parses ICMP header (8 bytes minimum)
6. Prints: source IP, dest IP, ICMP type and code

### Exercise 3: Track IPsec Sequence Numbers

Write a function that:
1. Takes a packet buffer
2. Skips to IPv4 header
3. Checks protocol is IPsec ESP (50)
4. Extracts SPI and sequence number
5. Maintains a static "last sequence number" variable
6. Detects and reports packet loss (gap in sequence numbers)

---

## Key Takeaways

1. **Pointer arithmetic is scaled by type** - `ptr++` moves by `sizeof(*ptr)` bytes
2. **`arr[i]` equals `*(arr + i)`** - Array notation is syntactic sugar
3. **Cast carefully** - Know what bytes you're interpreting as
4. **Always check bounds** - No automatic bounds checking in C
5. **Network byte order matters** - Use `ntohs()`, `ntohl()`, etc.
6. **Variable-length headers are the norm** - IP header, TCP header can vary
7. **Pointer patterns unlock packet parsing** - Master them and packet analysis is straightforward

Next: Once you're comfortable with pointer arithmetic, you're ready to define full network protocol structures and actually receive packets.
