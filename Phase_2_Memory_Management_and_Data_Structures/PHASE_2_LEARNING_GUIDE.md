# Phase 2: Memory Management & Data Structures for Networking
## Weeks 3-4: Building Your Packet Handling Toolkit

Welcome to Phase 2! You now understand C fundamentals and pointers conceptually. This phase takes that knowledge and applies it to real network engineering problems. By the end, you'll be able to build complex data structures that can handle thousands of packets per second.

---

## Why This Phase Matters

In Phase 1, you learned *what* pointers are. In Phase 2, you'll learn *how to use them effectively*. This is critical for network programming because:

**Network data is binary and structured:**
- An Ethernet frame has exactly 14 bytes of header, then a payload
- An IP header has 20+ bytes with specific field positions
- A TCP segment has 20+ bytes of header with flags, sequence numbers, etc.

**Python handles this abstraction:**
```python
# Python with scapy
packet = Ether(dst="ff:ff:ff:ff:ff:ff") / IP(dst="192.168.1.1") / ICMP()
packet.show()
```

**C requires you to understand memory layout:**
```c
// C with raw sockets
struct ethernet_frame eth;
struct ipv4_header ip;
struct icmp_header icmp;
// You must pack these correctly in a buffer
```

This phase teaches you to:
1. **Allocate and manage memory correctly** - No more garbage collection, you're in control
2. **Build packet structures** - Define C structs that match network protocol specs
3. **Use pointer arithmetic** - Navigate through packet data efficiently
4. **Implement data structures** - Ring buffers, linked lists for packet queues
5. **Avoid memory leaks** - Critical for long-running network applications

---

## Core Concepts

### 1. The Stack vs The Heap

This distinction is fundamental to C and network programming.

**Stack Memory:**
```c
void process_packet() {
    char buffer[1024];        // Stack allocation
    struct ethernet_frame eth; // Stack allocation

    // These are freed automatically when function returns
}
```

**Characteristics:**
- Automatically allocated when declared
- Automatically freed when variable goes out of scope
- Limited size (typically 1-8 MB, varies by OS)
- Very fast (just move a pointer)
- Not suitable for large buffers

**Heap Memory:**
```c
void process_many_packets() {
    char *buffer = malloc(1000000);  // Heap allocation

    // Must manually free when done
    free(buffer);
}
```

**Characteristics:**
- Manually allocated with `malloc()`
- Manually freed with `free()`
- Large size (typically several GB available)
- Slightly slower than stack
- **Your responsibility to track and free**

**For network programming:**
- Small fixed structures (Ethernet headers, IP headers) → Stack
- Large variable-size buffers (packet payload, capture rings) → Heap
- Arrays of packets → Heap

**Example - Packet Processing Loop:**
```c
// Stack: small, fixed-size structures
struct ethernet_frame eth_frame;
struct ipv4_header ip_header;

// Heap: variable-size packet buffer
unsigned char *packet_buffer = malloc(65535);  // Max Ethernet frame size

// Process packet...

free(packet_buffer);
```

### 2. malloc() and free() - Your New Best Friends

**malloc() - Memory Allocation**

```c
// Allocate space for 1000 integers
int *arr = malloc(1000 * sizeof(int));

// Allocate space for a packet header structure
struct ethernet_frame *eth = malloc(sizeof(struct ethernet_frame));

// Allocate a string buffer (100 bytes)
char *str = malloc(100);

// Allocate a 2D array of packet data (1000 packets, 1500 bytes each)
unsigned char *packets = malloc(1000 * 1500);  // Treat as linear buffer
```

**Key points about malloc():**
- Returns a pointer to the allocated memory (or NULL if allocation fails)
- The pointer points to the *first byte* of allocated memory
- You specify size in bytes
- Allocates on the heap, not the stack
- The allocated memory is uninitialized (contains garbage values)

**Always check for allocation failure:**
```c
int *arr = malloc(1000 * sizeof(int));
if (arr == NULL) {
    fprintf(stderr, "malloc failed! Out of memory?\n");
    return -1;
}
```

**calloc() - Cleaner Allocation**

`calloc()` is like `malloc()` but cleaner:
```c
// malloc version - allocate but leave uninitialized
int *arr = malloc(1000 * sizeof(int));

// calloc version - allocate AND zero-initialize
int *arr = calloc(1000, sizeof(int));  // All values set to 0
```

For networking, `calloc()` is often better because packets should start with zeros:
```c
// Good practice for packet buffers
unsigned char *packet = calloc(1500, 1);  // 1500 bytes, all zeros
```

**free() - Memory Deallocation**

```c
int *arr = malloc(1000 * sizeof(int));

// Use arr...

free(arr);      // Return memory to system
arr = NULL;     // Good practice: prevent accidental use-after-free
```

**Common mistakes with free():**
```c
// Mistake 1: Double free (calling free twice)
int *arr = malloc(100);
free(arr);
free(arr);  // CRASH! Undefined behavior!

// Mistake 2: Using after free
int *arr = malloc(100);
free(arr);
arr[0] = 5;  // CRASH! Memory was freed!

// Mistake 3: Freeing wrong pointer
int *arr = malloc(100);
int *ptr = arr + 10;
free(ptr);  // WRONG! Should free the original arr pointer

// Correct pattern:
int *arr = malloc(100);
if (arr == NULL) return -1;
// ... use arr ...
free(arr);
arr = NULL;
```

### 3. Pointer Arithmetic - Navigating Buffers

Pointers can be incremented/decremented to move through buffers.

**Basic Arithmetic:**
```c
int arr[5] = {10, 20, 30, 40, 50};
int *p = arr;

printf("%d\n", *p);         // 10 (first element)
printf("%d\n", *(p + 1));   // 20 (second element)
printf("%d\n", *(p + 2));   // 30 (third element)

p++;                         // Move to next integer
printf("%d\n", *p);         // 20

p += 2;                      // Move forward 2 integers
printf("%d\n", *p);         // 40
```

**Key insight:** Pointer arithmetic is scaled by type!
```c
int *p = malloc(4 * sizeof(int));
p++;     // Moves forward 4 bytes (size of int on most systems)

char *c = malloc(100);
c++;     // Moves forward 1 byte (size of char)

struct packet_header *pkt = malloc(sizeof(struct packet_header));
pkt++;   // Moves forward sizeof(struct packet_header) bytes
```

**For network packets:**
```c
// Ethernet frame: 14-byte header, variable payload
unsigned char *frame = malloc(1500);

// Point to different parts
unsigned char *ethernet_header = frame;           // Bytes 0-13
unsigned char *ip_header = frame + 14;           // Bytes 14+
unsigned char *payload = frame + 14 + 20;        // Bytes 34+ (assuming 20-byte IP header)

printf("Dest MAC: %02x:%02x:%02x\n", ethernet_header[0], ethernet_header[1], ethernet_header[2]);
printf("IP version: %d\n", (ip_header[0] >> 4) & 0x0F);
```

**Practical example - Parsing a packet:**
```c
void parse_packet(unsigned char *packet, int packet_len) {
    // Point to different headers
    struct ethernet_frame *eth = (struct ethernet_frame *)packet;
    struct ipv4_header *ip = (struct ipv4_header *)(packet + 14);

    printf("Ethernet type: 0x%04x\n", ntohs(eth->ethertype));
    printf("IP src: %s\n", inet_ntoa(ip->src_addr));
    printf("Payload starts at offset: %d\n", 14 + (ip->ihl * 4));
}
```

### 4. Structures for Network Protocols

Network protocols define exact byte layouts. C structs must match these layouts precisely.

**Ethernet Frame Header (RFC 802.3):**
```
Bytes 0-5:    Destination MAC address (6 bytes)
Bytes 6-11:   Source MAC address (6 bytes)
Bytes 12-13:  EtherType (2 bytes, big-endian)
Bytes 14+:    Payload
```

**C struct:**
```c
struct ethernet_frame {
    unsigned char dest_mac[6];
    unsigned char src_mac[6];
    unsigned short ethertype;  // Big-endian!
};
```

**IPv4 Header (RFC 791):**
```
Byte 0:       Version (4 bits) + IHL (4 bits)
Byte 1:       DSCP (6 bits) + ECN (2 bits)
Bytes 2-3:    Total Length (big-endian)
Bytes 4-5:    Identification (big-endian)
Bytes 6-7:    Flags (3 bits) + Fragment Offset (13 bits)
Byte 8:       TTL
Byte 9:       Protocol
Bytes 10-11:  Header Checksum (big-endian)
Bytes 12-15:  Source IP (big-endian)
Bytes 16-19:  Destination IP (big-endian)
Bytes 20+:    Options (if IHL > 5), then payload
```

**C struct:**
```c
struct ipv4_header {
    unsigned char version_ihl;         // Version(4) + IHL(4)
    unsigned char dscp_ecn;            // DSCP(6) + ECN(2)
    unsigned short total_length;       // Big-endian
    unsigned short identification;     // Big-endian
    unsigned short flags_frag_offset;  // Flags(3) + Offset(13), big-endian
    unsigned char ttl;
    unsigned char protocol;
    unsigned short header_checksum;    // Big-endian
    struct in_addr src_addr;           // Defined in <netinet/in.h>, 4 bytes
    struct in_addr dest_addr;          // 4 bytes
    // Note: No options field in basic struct; handle separately
};
```

**Important: Structure Packing**

By default, C may add padding to align structs. This breaks network protocol parsing:

```c
struct bad_example {
    char a;           // 1 byte
    // 3 bytes padding here!
    int b;            // 4 bytes
    char c;           // 1 byte
    // 3 bytes padding here!
};
// sizeof(bad_example) = 12, but should be 6!

// Solution: Use packing attribute (GCC/Clang)
struct good_example {
    char a;
    int b;
    char c;
} __attribute__((packed));
// sizeof(good_example) = 6 ✓
```

**For network headers, always use `__attribute__((packed))`:**
```c
struct ethernet_frame {
    unsigned char dest_mac[6];
    unsigned char src_mac[6];
    unsigned short ethertype;
} __attribute__((packed));

// Now sizeof(ethernet_frame) == 14 bytes guaranteed
```

### 5. Unions for Protocol Parsing

Sometimes you need to view the same data in different ways. Use `union`:

```c
// View IPv4 address as either 4 bytes or a 32-bit integer
union ipv4_addr {
    unsigned char bytes[4];
    unsigned int addr;  // 32-bit representation
};

union ipv4_addr ip;
ip.bytes[0] = 192;
ip.bytes[1] = 168;
ip.bytes[2] = 1;
ip.bytes[3] = 1;

printf("As integer: 0x%08x\n", ip.addr);  // Will show combined value
```

**For network flags (using bitfields):**
```c
struct tcp_flags {
    unsigned char reserved : 3;  // 3 bits
    unsigned char nonce : 1;     // 1 bit
    unsigned char cwr : 1;       // 1 bit
    unsigned char ece : 1;       // 1 bit
    unsigned char urg : 1;       // 1 bit
    unsigned char ack : 1;       // 1 bit
} __attribute__((packed));
```

**For IPsec/MACsec packet analysis:**
```c
// IPsec ESP header
struct ipsec_esp_header {
    unsigned int spi;              // Security Parameter Index
    unsigned int sequence_number;  // THE critical field you're tracking!
    // Followed by encrypted payload
} __attribute__((packed));

// MACsec header
struct macsec_header {
    unsigned char tci_an;  // TCI byte with packet number indicator
    // Followed by various fields including packet number
    unsigned short packet_number;
} __attribute__((packed));
```

### 6. Bit Manipulation for Protocol Fields

Network protocols use bits within bytes. You need bitwise operations:

```c
// Extract IP version (top 4 bits) from first byte
unsigned char version_ihl = 0x45;  // Version 4, IHL 5
unsigned char version = (version_ihl >> 4) & 0x0F;  // Extract top 4 bits
unsigned char ihl = version_ihl & 0x0F;             // Extract bottom 4 bits

printf("Version: %d, IHL: %d\n", version, ihl);  // "Version: 4, IHL: 5"

// Extract TCP flags from flags byte
unsigned char flags = 0x18;  // Binary: 00011000 (PSH=1, ACK=1)
unsigned char fin = (flags >> 0) & 1;
unsigned char syn = (flags >> 1) & 1;
unsigned char rst = (flags >> 2) & 1;
unsigned char psh = (flags >> 3) & 1;
unsigned char ack = (flags >> 4) & 1;
unsigned char urg = (flags >> 5) & 1;

printf("ACK=%d, PSH=%d\n", ack, psh);  // "ACK=1, PSH=1"

// Set flags
unsigned char tcp_flags = 0;
tcp_flags |= (1 << 4);  // Set ACK flag
tcp_flags |= (1 << 3);  // Set PSH flag
printf("TCP flags byte: 0x%02x\n", tcp_flags);  // "TCP flags byte: 0x18"
```

**For IPsec/MACsec:**
```c
// Check if IPsec SPI matches what we're tracking
struct ipsec_esp_header *esp = (struct ipsec_esp_header *)packet_data;
unsigned int spi = ntohl(esp->spi);
unsigned int seq_num = ntohl(esp->sequence_number);

if (spi == MY_TRACKED_SPI) {
    printf("Tracked packet: SPI=0x%x, Seq=%u\n", spi, seq_num);
}
```

### 7. Linked Lists for Packet Queues

Dynamic lists are crucial for buffering packets.

**Basic Node Structure:**
```c
struct packet_node {
    unsigned char *data;           // Pointer to packet data
    int length;                    // Packet length
    unsigned long timestamp;       // When packet was captured
    struct packet_node *next;      // Pointer to next node
};
```

**Linked List Operations:**

```c
// Create a new node
struct packet_node *create_node(unsigned char *packet, int len) {
    struct packet_node *node = malloc(sizeof(struct packet_node));
    if (node == NULL) return NULL;

    node->data = malloc(len);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    memcpy(node->data, packet, len);  // Copy packet data
    node->length = len;
    node->timestamp = 0;  // Set by caller
    node->next = NULL;

    return node;
}

// Add to front of list
void prepend_packet(struct packet_node **head, struct packet_node *new_node) {
    new_node->next = *head;
    *head = new_node;
}

// Add to end of list
void append_packet(struct packet_node **head, struct packet_node *new_node) {
    if (*head == NULL) {
        *head = new_node;
        return;
    }

    struct packet_node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

// Free entire list
void free_packet_list(struct packet_node *head) {
    while (head != NULL) {
        struct packet_node *next = head->next;
        free(head->data);
        free(head);
        head = next;
    }
}
```

**For packet loss tracking with IPsec:**
```c
struct ipsec_packet {
    unsigned int spi;
    unsigned int sequence_number;
    unsigned long timestamp;
    struct ipsec_packet *next;
};

// Track packets we've seen
struct ipsec_packet *seen_packets = NULL;

void track_ipsec_packet(unsigned int spi, unsigned int seq) {
    struct ipsec_packet *node = malloc(sizeof(struct ipsec_packet));
    node->spi = spi;
    node->sequence_number = seq;
    node->timestamp = get_current_time();
    node->next = seen_packets;
    seen_packets = node;
}

// Check for gaps in sequence numbers
void detect_losses() {
    unsigned int last_seq = 0;
    for (struct ipsec_packet *p = seen_packets; p != NULL; p = p->next) {
        if (p->sequence_number != last_seq + 1) {
            printf("Potential packet loss detected!\n");
            printf("Expected: %u, Got: %u\n", last_seq + 1, p->sequence_number);
        }
        last_seq = p->sequence_number;
    }
}
```

### 8. Ring Buffers for High-Performance Capture

Ring buffers are better than linked lists for capturing packets at high speed.

**Why Ring Buffers?**
- Fixed memory allocation (no malloc/free during capture)
- No pointer chasing (faster access)
- Automatic wraparound when full
- Easy to implement zero-copy semantics

**Simple Ring Buffer Implementation:**

```c
struct ring_buffer {
    unsigned char **packets;       // Array of packet pointers
    int *packet_lengths;           // Array of packet lengths
    int capacity;                  // Total slots
    int write_index;               // Next write position
    int read_index;                // Next read position
    int count;                     // Packets currently in buffer
};

// Initialize ring buffer
struct ring_buffer *ring_buffer_create(int capacity) {
    struct ring_buffer *rb = malloc(sizeof(struct ring_buffer));
    rb->packets = malloc(capacity * sizeof(unsigned char *));
    rb->packet_lengths = malloc(capacity * sizeof(int));
    rb->capacity = capacity;
    rb->write_index = 0;
    rb->read_index = 0;
    rb->count = 0;

    for (int i = 0; i < capacity; i++) {
        rb->packets[i] = malloc(1500);  // Max packet size
    }

    return rb;
}

// Write a packet to ring buffer
int ring_buffer_write(struct ring_buffer *rb, unsigned char *packet, int len) {
    if (rb->count >= rb->capacity) {
        return -1;  // Buffer full
    }

    memcpy(rb->packets[rb->write_index], packet, len);
    rb->packet_lengths[rb->write_index] = len;
    rb->write_index = (rb->write_index + 1) % rb->capacity;
    rb->count++;

    return 0;
}

// Read a packet from ring buffer
int ring_buffer_read(struct ring_buffer *rb, unsigned char *out_packet, int *out_len) {
    if (rb->count == 0) {
        return -1;  // Buffer empty
    }

    memcpy(out_packet, rb->packets[rb->read_index], rb->packet_lengths[rb->read_index]);
    *out_len = rb->packet_lengths[rb->read_index];
    rb->read_index = (rb->read_index + 1) % rb->capacity;
    rb->count--;

    return 0;
}

// Cleanup
void ring_buffer_free(struct ring_buffer *rb) {
    for (int i = 0; i < rb->capacity; i++) {
        free(rb->packets[i]);
    }
    free(rb->packets);
    free(rb->packet_lengths);
    free(rb);
}
```

---

## Phase 2 Projects

### Project 4: Packet Buffer Pool

**Goal:** Implement memory pooling for fast packet allocation/deallocation

**Why:** Repeatedly calling malloc/free for millions of packets is slow. Pre-allocate a pool and reuse buffers.

**Requirements:**
- Create a pool of 1000 pre-allocated packet buffers (1500 bytes each)
- Implement `get_buffer()` to retrieve unused buffer
- Implement `return_buffer()` to return buffer to pool
- Track which buffers are in use
- Measure allocation time with and without pool

**What you'll learn:**
- Memory pooling patterns
- Performance impact of malloc/free
- Practical optimization for network applications

**Skills developed:**
- Custom memory management
- Data structure design
- Performance measurement

### Project 5: Ring Buffer Implementation

**Goal:** Build a thread-safe ring buffer for packet queuing

**Requirements:**
- Implement circular buffer with fixed capacity
- Write packets to buffer (FIFO)
- Read packets from buffer
- Handle wraparound correctly
- Count packets in buffer
- Basic synchronization (if multi-threaded)

**What you'll learn:**
- Circular buffer algorithm
- Modulo arithmetic for wraparound
- Thread-safe data structures (basics)
- High-performance queue design

**Skills developed:**
- Pointer arithmetic and circular logic
- Synchronization primitives (mutex, atomic operations)
- Data structure implementation

### Project 6: Protocol Stack Data Structures

**Goal:** Define structs for Ethernet, IP, TCP, UDP, ICMP, and ARPheaders

**Requirements:**
- Create `ethernet_frame` struct (14 bytes)
- Create `ipv4_header` struct (20+ bytes, handle variable IHL)
- Create `tcp_header` struct (20+ bytes, handle variable options)
- Create `udp_header` struct (8 bytes)
- Create `icmp_header` struct (varies by type)
- Create `arp_packet` struct (28 bytes typical)
- All structs use `__attribute__((packed))`
- Write functions to print each header in human-readable format
- Parse a real packet capture file (pcap format) and display headers

**What you'll learn:**
- Network protocol specifications (RFCs)
- Struct design and packing
- Reading protocol documentation
- Casting and pointer interpretation
- Working with binary data

**Skills developed:**
- Protocol knowledge
- Struct definition
- RFC reading
- Binary data analysis

### Project 7: Checksum Calculator

**Goal:** Implement IP and TCP checksum algorithms

**Requirements:**
- Implement IP header checksum (RFC 791 one's complement)
- Implement TCP checksum (RFC 793 one's complement)
- Handle pseudo-header for TCP
- Validate checksums on received packets
- Correct invalid checksums

**Background:**
IP and TCP checksums use one's complement arithmetic (sum all 16-bit words, take complement).

```c
unsigned short calculate_ip_checksum(struct ipv4_header *header) {
    unsigned int sum = 0;
    unsigned short *ptr = (unsigned short *)header;
    int len = (header->version_ihl & 0x0F) * 4;  // IHL in 32-bit words

    // Zero out checksum field
    header->header_checksum = 0;

    // Sum all 16-bit words
    while (len > 0) {
        sum += *ptr++;
        len -= 2;
    }

    // Handle overflow
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum;  // One's complement
}
```

**What you'll learn:**
- Checksum algorithms
- One's complement arithmetic
- Pseudo-headers
- Packet validation

**Skills developed:**
- Algorithm implementation
- Network standards compliance
- Packet integrity verification

---

## Common Memory Mistakes and How to Avoid Them

### Mistake 1: Memory Leak

```c
// BAD: Allocating in a loop without freeing
void process_packets(int num_packets) {
    for (int i = 0; i < num_packets; i++) {
        char *packet = malloc(1500);
        // Process packet...
        // OOPS! Never freed!
    }
}

// GOOD:
void process_packets(int num_packets) {
    for (int i = 0; i < num_packets; i++) {
        char *packet = malloc(1500);
        // Process packet...
        free(packet);
    }
}
```

### Mistake 2: Buffer Overflow

```c
// BAD: Writing beyond allocated memory
char *buf = malloc(100);
strcpy(buf, "This is a very long string that exceeds 100 bytes");  // CRASH!

// GOOD:
char *buf = malloc(100);
strncpy(buf, "Short string", 99);
buf[99] = '\0';
free(buf);
```

### Mistake 3: Use-After-Free

```c
// BAD:
char *packet = malloc(1500);
free(packet);
packet[0] = 'A';  // CRASH! Memory was freed!

// GOOD:
char *packet = malloc(1500);
free(packet);
packet = NULL;  // Prevent accidental reuse
// Don't use packet after this
```

### Mistake 4: Double Free

```c
// BAD:
char *packet = malloc(1500);
free(packet);
free(packet);  // CRASH! Undefined behavior!

// GOOD:
char *packet = malloc(1500);
free(packet);
packet = NULL;
// Don't free again
```

### Mistake 5: Forgetting to Check malloc()

```c
// BAD:
int *arr = malloc(1000000000);  // Might fail!
arr[0] = 5;  // CRASH if malloc returned NULL!

// GOOD:
int *arr = malloc(1000000000);
if (arr == NULL) {
    fprintf(stderr, "malloc failed\n");
    return -1;
}
arr[0] = 5;
free(arr);
```

---

## Debugging Tools for Memory Issues

### valgrind - Find Memory Leaks

```bash
# Compile with -g for debug info
gcc -g my_program.c -o my_program

# Run with valgrind
valgrind --leak-check=full ./my_program

# Output shows:
# LEAK SUMMARY:
#     definitely lost: 1,500 bytes in 1 blocks
#     indirectly lost: 0 bytes in 0 blocks
#     possibly lost: 0 bytes in 0 blocks
```

### gdb - Debug Memory Access

```bash
gdb ./my_program

(gdb) break main
(gdb) run
(gdb) print arr
(gdb) print &arr
(gdb) print *ptr
(gdb) watch *ptr          # Stop when memory at ptr changes
(gdb) continue
```

### Address Sanitizer (ASAN)

```bash
# Compile with ASAN enabled
gcc -fsanitize=address -g my_program.c -o my_program

# Run normally - ASAN will catch errors
./my_program
```

---

## Phase 2 Learning Timeline

### Week 3: Memory Management and Pointers

**Day 1-2: Stack vs Heap**
- Understand automatic vs manual memory management
- Learn malloc/calloc/free
- Complete Project 4 (Packet Buffer Pool)

**Day 3-5: Pointer Arithmetic**
- Practice pointer arithmetic on arrays and structs
- Navigate packet data with pointers
- Write parsing code for packet buffers

**Day 6-7: Debugging Memory Issues**
- Use valgrind to find leaks
- Use gdb to inspect memory
- Fix memory bugs in your code

### Week 4: Data Structures for Networking

**Day 1-2: Network Protocol Structures**
- Read RFC 791 (IPv4), 793 (TCP), 792 (ICMP), 826 (ARP)
- Design structs for each protocol
- Complete Project 6 (Protocol Stack Data Structures)

**Day 3-4: Advanced Data Structures**
- Implement linked lists
- Implement ring buffers
- Complete Project 5 (Ring Buffer Implementation)

**Day 5-7: Checksums and Packet Validation**
- Understand one's complement checksum algorithm
- Implement checksum calculation and validation
- Complete Project 7 (Checksum Calculator)

---

## Self-Assessment: Ready for Phase 3?

You're ready if you can:
- [ ] Explain stack vs heap memory
- [ ] Use malloc/calloc/free without crashes
- [ ] Implement pointer arithmetic to parse binary data
- [ ] Define packed structs for network protocols
- [ ] Use gdb and valgrind to debug memory issues
- [ ] Implement a simple linked list
- [ ] Implement a ring buffer
- [ ] Calculate IP and TCP checksums correctly
- [ ] Parse a real packet capture and display headers
- [ ] Track memory usage and eliminate leaks

If any of these seem unclear, spend more time on that concept before Phase 3!

---

## Key Takeaways

1. **Stack vs Heap:** Understanding when to use each is fundamental to C programming
2. **Pointer Arithmetic:** Mastering `*p`, `p++`, `p + n`, etc. unlocks packet parsing
3. **Structs and Packing:** Network protocols require precise byte layouts - use `__attribute__((packed))`
4. **Memory Management:** malloc/free put YOU in control - this is powerful but requires discipline
5. **Data Structures:** Linked lists and ring buffers are building blocks for packet processing
6. **Tools Matter:** valgrind and gdb are your friends for debugging memory issues

Phase 3 will teach you to actually send and receive packets using raw sockets. You'll use these data structures and memory management patterns constantly.

Good luck! You're building the foundation for high-performance network tools.
