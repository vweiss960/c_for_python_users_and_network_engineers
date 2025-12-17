# Memory Management Deep Dive
## From malloc/free to Packet Buffers

This guide provides detailed, practical explanations of C's manual memory management system and how it applies to network programming.

---

## The Heap - Your Workshop

Think of the heap as a large workshop where you can request blocks of memory. Unlike the stack (which has fixed space), the heap is large and flexible—perfect for network programming where you don't know packet sizes in advance.

### How the Heap Works

```
Before:  [Allocated: 100 bytes] [Free: 500 bytes] [Allocated: 200 bytes] [Free: 5000 bytes]
                      |                                |
                      ptr1                            ptr2

malloc(150):
         [Allocated: 100] [FREE: 500] <- Allocate 150 here
                                    |
                                    ptr3

Result: [Allocated: 100] [Allocated: 150] [Free: 350] [Allocated: 200] [Free: 5000]
                          |
                          ptr3
```

The OS heap manager tracks which blocks are free and which are allocated. When you call `malloc(size)`:
1. Find a free block at least `size` bytes
2. Mark it as allocated
3. Return a pointer to the start
4. When you call `free(ptr)`, mark that block as free again

### malloc() - Detailed Behavior

**Basic syntax:**
```c
void *malloc(size_t size);
```

**Returns:**
- A pointer to newly allocated memory (on success)
- NULL if allocation failed (out of memory)
- The memory is uninitialized (contains garbage)

**Important notes:**
- malloc returns `void *` (generic pointer) - C will implicitly cast it
- You specify size in bytes - must match the type of data
- The pointer points to the first byte of allocated memory

**Examples with different types:**

```c
// For integers
int *ptr_int = malloc(sizeof(int));  // Typical: 4 bytes
*ptr_int = 42;

// For arrays
int *arr = malloc(100 * sizeof(int));  // 400 bytes on 32-bit int systems
arr[0] = 10;
arr[99] = 20;

// For structs
struct ethernet_frame *eth = malloc(sizeof(struct ethernet_frame));
eth->ethertype = 0x0800;

// For character strings
char *str = malloc(256);  // Room for 255 chars + null terminator
strcpy(str, "Hello");

// For packet buffers
unsigned char *packet = malloc(1500);  // Max Ethernet frame
```

**The sizeof() operator is critical:**

```c
// WRONG: allocating 8 bytes when you need 400
int *arr = malloc(100);  // Only 100 bytes, not 100 integers!
arr[10] = 5;  // CRASH!

// RIGHT: allocating 400 bytes for 100 integers
int *arr = malloc(100 * sizeof(int));
arr[10] = 5;  // Safe
```

### calloc() - Initialized Allocation

`calloc()` is malloc's safer cousin - it zero-initializes memory:

```c
void *calloc(size_t num_items, size_t item_size);
```

**Difference:**
```c
// malloc: uninitialized (contains garbage)
int *arr1 = malloc(100 * sizeof(int));
printf("%d\n", arr1[0]);  // Undefined! Could be any value

// calloc: zero-initialized
int *arr2 = calloc(100, sizeof(int));
printf("%d\n", arr2[0]);  // 0 (guaranteed)
```

**For network programming, calloc is often better:**
```c
// Packet buffers should start clean
unsigned char *packet = calloc(1500, 1);  // 1500 bytes, all zeros

// Tracking structures should be initialized
struct ipsec_packet *tracker = calloc(1000, sizeof(struct ipsec_packet));
// All fields are zero initially
```

**Performance note:** calloc does extra work (zero-filling), so if you don't need zeroed memory, malloc is faster.

### free() - Returning Memory

```c
void free(void *ptr);
```

**When you call free():**
1. The memory at `ptr` is marked as free in the heap manager
2. The heap manager can now reuse that memory for future allocations
3. Your pointer is now invalid and should not be used

**Correct pattern:**
```c
int *arr = malloc(1000 * sizeof(int));
if (arr == NULL) {
    perror("malloc failed");
    return -1;
}

// Use arr...
for (int i = 0; i < 1000; i++) {
    arr[i] = i * 2;
}

free(arr);
arr = NULL;  // Good practice
```

### Memory Leak - The Most Common Mistake

A memory leak occurs when you allocate memory but forget to free it:

```c
// LEAK: memory allocated but never freed
void process_packets(int num) {
    for (int i = 0; i < num; i++) {
        unsigned char *packet = malloc(1500);
        // Process packet...
        // FORGOT TO FREE!
    }
}
// After this loop, 1500 * num bytes are lost
```

**Why this is bad for network applications:**
- Network programs run continuously (days, weeks, months)
- A small leak of 1500 bytes per packet × 1 million packets/hour = 1.5 GB/hour
- After 1 week, your program has consumed 252 GB - it crashes
- This silently accumulates; there's no garbage collector to save you

**Detecting leaks with valgrind:**
```bash
gcc -g my_program.c -o my_program
valgrind --leak-check=full ./my_program

# Output shows:
LEAK SUMMARY:
    definitely lost: 1,500,000 bytes in 1,000,000 blocks
```

**Prevention:**
- Always pair malloc with free in the same function (or document who owns the pointer)
- Use consistent patterns
- Test with valgrind regularly
- Allocate once, reuse many times (memory pools, ring buffers)

### realloc() - Resizing Allocations

```c
void *realloc(void *ptr, size_t new_size);
```

**Extends or shrinks an existing allocation:**
```c
int *arr = malloc(100 * sizeof(int));

// Need more space
int *new_arr = realloc(arr, 200 * sizeof(int));
if (new_arr == NULL) {
    // realloc failed, but arr still valid
    free(arr);
    return -1;
}
arr = new_arr;  // Use resized array

free(arr);
```

**Important:** realloc may move your data to a new location, so always update your pointer.

---

## Advanced Memory Management Patterns

### Pattern 1: Memory Pools

For high-performance packet processing, pre-allocate a pool of buffers:

```c
#define PACKET_POOL_SIZE 1000
#define PACKET_SIZE 1500

struct packet_pool {
    unsigned char *buffers[PACKET_POOL_SIZE];
    int available_count;
    int available_indices[PACKET_POOL_SIZE];
};

struct packet_pool *pool_create() {
    struct packet_pool *pool = malloc(sizeof(struct packet_pool));

    // Pre-allocate all buffers
    for (int i = 0; i < PACKET_POOL_SIZE; i++) {
        pool->buffers[i] = malloc(PACKET_SIZE);
        if (pool->buffers[i] == NULL) {
            // Handle allocation failure
            return NULL;
        }
        pool->available_indices[i] = i;
    }

    pool->available_count = PACKET_POOL_SIZE;
    return pool;
}

unsigned char *pool_get_buffer(struct packet_pool *pool) {
    if (pool->available_count == 0) {
        return NULL;  // No buffers available
    }

    int idx = pool->available_indices[--pool->available_count];
    return pool->buffers[idx];
}

void pool_return_buffer(struct packet_pool *pool, unsigned char *buffer) {
    if (pool->available_count < PACKET_POOL_SIZE) {
        // Find which buffer this is
        for (int i = 0; i < PACKET_POOL_SIZE; i++) {
            if (pool->buffers[i] == buffer) {
                pool->available_indices[pool->available_count++] = i;
                return;
            }
        }
    }
}

void pool_free(struct packet_pool *pool) {
    for (int i = 0; i < PACKET_POOL_SIZE; i++) {
        free(pool->buffers[i]);
    }
    free(pool);
}
```

**Advantages:**
- No malloc/free overhead during packet processing
- Predictable performance (no GC pauses)
- Memory is pre-allocated, so no surprise failures
- Reuse is cache-friendly

**Usage:**
```c
struct packet_pool *pool = pool_create();

// Process packets
while (receiving_packets) {
    unsigned char *buf = pool_get_buffer(pool);
    if (buf == NULL) {
        fprintf(stderr, "No buffers available\n");
        continue;
    }

    // Receive packet into buf
    int len = receive_packet(buf);

    // Process...

    pool_return_buffer(pool, buf);
}

pool_free(pool);
```

### Pattern 2: Dynamic Arrays with Growth

When you don't know how many packets you'll capture:

```c
struct dynamic_packet_array {
    unsigned char **packets;
    int *lengths;
    int count;
    int capacity;
};

void dpa_append(struct dynamic_packet_array *arr, unsigned char *packet, int len) {
    // Grow if needed
    if (arr->count >= arr->capacity) {
        int new_capacity = arr->capacity * 2;  // Double size

        unsigned char **new_packets = realloc(arr->packets, new_capacity * sizeof(unsigned char *));
        int *new_lengths = realloc(arr->lengths, new_capacity * sizeof(int));

        if (new_packets == NULL || new_lengths == NULL) {
            fprintf(stderr, "realloc failed\n");
            return;
        }

        arr->packets = new_packets;
        arr->lengths = new_lengths;
        arr->capacity = new_capacity;
    }

    // Copy packet
    arr->packets[arr->count] = malloc(len);
    memcpy(arr->packets[arr->count], packet, len);
    arr->lengths[arr->count] = len;
    arr->count++;
}

void dpa_free(struct dynamic_packet_array *arr) {
    for (int i = 0; i < arr->count; i++) {
        free(arr->packets[i]);
    }
    free(arr->packets);
    free(arr->lengths);
    free(arr);
}
```

### Pattern 3: Pointer Ownership

For complex programs, clearly document who owns each pointer:

```c
// Function creates memory - CALLER must free
struct ipsec_packet_info *parse_ipsec_packet(unsigned char *raw_packet) {
    struct ipsec_packet_info *info = malloc(sizeof(struct ipsec_packet_info));
    // ... parse ...
    return info;  // CALLER owns this
}

// Function uses memory - CALLER must keep it valid
void print_ipsec_info(struct ipsec_packet_info *info) {
    printf("SPI: 0x%x\n", info->spi);
    // No freeing here - we don't own it
}

// Usage:
struct ipsec_packet_info *info = parse_ipsec_packet(raw_data);
print_ipsec_info(info);
free(info);  // CALLER responsibility
```

---

## Memory Layout and Alignment

### How Data is Stored in Memory

```c
char c = 'A';       // 1 byte: [0x41]
int i = 0x12345678; // 4 bytes: [0x78, 0x56, 0x34, 0x12] (little-endian)
short s = 0x1234;   // 2 bytes: [0x34, 0x12] (little-endian)
```

**Memory addresses:**
```c
Address:  0x1000  0x1001  0x1002  0x1003  0x1004  0x1005  0x1006
Value:    [0x41]  [0x78]  [0x56]  [0x34]  [0x12]  [0x34]  [0x12]
           ^char   ^int starts        ^ends   ^short starts  ^ends
```

### Structure Padding and Alignment

By default, C adds padding to align struct members:

```c
struct example {
    char c;        // 1 byte at offset 0
    // 3 bytes padding (automatic)
    int i;         // 4 bytes at offset 4 (aligned to 4-byte boundary)
    short s;       // 2 bytes at offset 8
    // 2 bytes padding
};
// Total: 12 bytes (but only 7 bytes of actual data!)
```

**For network protocols, this breaks packet parsing:**

```c
// WRONG - padding breaks binary compatibility
struct ethernet_frame {
    unsigned char dest_mac[6];
    unsigned char src_mac[6];
    unsigned short ethertype;
};
// Expected size: 14 bytes
// Actual size: might be 16 bytes!

// CORRECT - use packed attribute
struct ethernet_frame {
    unsigned char dest_mac[6];
    unsigned char src_mac[6];
    unsigned short ethertype;
} __attribute__((packed));
// Size: exactly 14 bytes
```

**Verify with sizeof:**
```c
printf("ethernet_frame size: %lu\n", sizeof(struct ethernet_frame));
// Output: ethernet_frame size: 14
```

---

## Common Pitfalls and Solutions

### Pitfall 1: Stack Overflow with Large Arrays

```c
// BAD: Allocate 10MB on stack
void process_packet() {
    unsigned char large_buffer[10000000];  // CRASH! Stack too small
    // ...
}

// GOOD: Allocate on heap
void process_packet() {
    unsigned char *large_buffer = malloc(10000000);
    if (large_buffer == NULL) {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    // ...
    free(large_buffer);
}
```

### Pitfall 2: Uninitialized Pointers

```c
// BAD: Pointer points to garbage
int *ptr;
*ptr = 5;  // Crash! ptr contains random address

// GOOD: Always initialize
int *ptr = malloc(sizeof(int));
if (ptr == NULL) {
    return -1;
}
*ptr = 5;
free(ptr);
```

### Pitfall 3: Dangling Pointers

```c
// BAD: Pointer to freed memory
int *ptr = malloc(100);
free(ptr);
ptr[0] = 5;  // Crash! Memory was freed

// GOOD:
int *ptr = malloc(100);
free(ptr);
ptr = NULL;
// Now if we accidentally use ptr, it's obvious (NULL dereference)
```

### Pitfall 4: Mixed Stack/Heap Ownership

```c
// BAD: Returning pointer to stack memory
int *create_array() {
    int arr[10] = {1, 2, 3};  // Stack memory
    return arr;  // WRONG! arr goes out of scope
}

int *arr = create_array();
arr[0] = 5;  // Crash! arr points to freed stack memory

// GOOD: Return heap memory
int *create_array() {
    int *arr = malloc(10 * sizeof(int));
    arr[0] = 1;
    return arr;  // Caller must free
}

int *arr = create_array();
arr[0] = 5;  // Safe
free(arr);
```

---

## Memory Debugging Tools

### valgrind - Memory Error Detector

```bash
# Compile with debug symbols
gcc -g -Wall my_program.c -o my_program

# Run with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./my_program
```

**Output example:**
```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 1,500 bytes in 1 blocks
==12345==   total heap alloc+free: 15,000 bytes in 10 blocks
==12345==
==12345== LEAK SUMMARY:
==12345==    definitely lost: 1,500 bytes in 1 blocks
```

**Leak types:**
- **definitely lost**: You forgot to free (fix these)
- **indirectly lost**: Pointed to by a definitely lost block
- **possibly lost**: Might be leaks (often false positives with complex pointers)
- **still reachable**: Allocated but not freed (acceptable for program-lifetime objects)

### Address Sanitizer (ASAN)

```bash
# Compile with ASAN
gcc -fsanitize=address -g my_program.c -o my_program

# Run normally
./my_program

# ASAN prints errors like:
#==12345==ERROR: AddressSanitizer: heap-buffer-overflow
```

Faster than valgrind, built into the binary.

### gdb - Interactive Debugging

```bash
gdb ./my_program

(gdb) break main
(gdb) run
(gdb) print ptr              # Print pointer value
(gdb) print *ptr             # Dereference pointer
(gdb) print &variable        # Address of variable
(gdb) info locals            # Show all local variables with addresses
(gdb) x/4x ptr               # Examine 4 hex values at ptr
(gdb) watch *ptr             # Stop when memory changes
```

### strace - System Call Tracing

```bash
strace -e trace=mmap,munmap ./my_program
```

Shows all memory allocation/deallocation system calls.

---

## Best Practices for Network Programming Memory Management

1. **Check all malloc results:**
   ```c
   char *buf = malloc(1500);
   if (buf == NULL) {
       return -1;  // Or handle error appropriately
   }
   ```

2. **Use pools for high-performance capture:**
   - Pre-allocate packet buffers
   - Reuse instead of malloc/free
   - No surprise allocation failures during packet processing

3. **Always use __attribute__((packed)) for network structs:**
   ```c
   struct my_header {
       // ...
   } __attribute__((packed));
   ```

4. **Document pointer ownership:**
   - Who allocates?
   - Who must free?
   - Who owns the memory while being used?

5. **Initialize before use:**
   - Use calloc() for structures that should be zeroed
   - Explicitly set fields before reading

6. **Test with valgrind regularly:**
   - Make it part of your build/test process
   - Catch leaks early before they cause crashes in production

7. **Avoid malloc/free in hot loops:**
   - Pre-allocate everything
   - Reuse buffers with pools or ring buffers
   - Keep packet processing path malloc-free

---

## Key Takeaways

1. **malloc gets memory, free returns it** - Both are your responsibility
2. **Check malloc return value** - Allocation can fail
3. **Use sizeof() operator** - Avoid hardcoding sizes
4. **Use __attribute__((packed))** - For network protocol structures
5. **Memory pools beat malloc/free** - For high-performance packet processing
6. **valgrind is your friend** - Use it religiously
7. **Document ownership** - Who allocates? Who frees?
8. **Prevent leaks early** - They compound in long-running network apps

This foundation is critical for Phase 3 and beyond, where you'll be capturing and processing millions of packets. Memory management mistakes at that scale are catastrophic.
