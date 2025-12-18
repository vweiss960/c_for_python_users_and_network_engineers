# Packet Header Parser - Makefile
# Build automation for the parser project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -std=c99
LDFLAGS =

# Target binaries
PARSER = packet_parser
PARSER_SOL = parser_solution
GENERATOR = gen_packet

# Source files
STARTER_SRC = 02_starter.c
SOLUTION_SRC = 02_c_solution.c
GENERATOR_SRC = generate_sample_packet.c

# Test data
SAMPLE_PACKETS = sample_packet.bin sample_udp_packet.bin minimal_packet.bin

# Build all targets
all: $(PARSER) $(PARSER_SOL) $(GENERATOR) $(SAMPLE_PACKETS)

# Build starter parser (your solution)
$(PARSER): $(STARTER_SRC)
	$(CC) $(CFLAGS) -o $(PARSER) $(STARTER_SRC) $(LDFLAGS)
	@echo "Built: $(PARSER)"

# Build reference solution
$(PARSER_SOL): $(SOLUTION_SRC)
	$(CC) $(CFLAGS) -o $(PARSER_SOL) $(SOLUTION_SRC) $(LDFLAGS)
	@echo "Built: $(PARSER_SOL)"

# Build packet generator
$(GENERATOR): $(GENERATOR_SRC)
	$(CC) $(CFLAGS) -o $(GENERATOR) $(GENERATOR_SRC) $(LDFLAGS)
	@echo "Built: $(GENERATOR)"

# Generate sample packets
$(SAMPLE_PACKETS): $(GENERATOR)
	./$(GENERATOR)

# Run tests with the solution
test: $(PARSER_SOL) $(SAMPLE_PACKETS)
	@echo "\n=== Testing with solution ==="
	./$(PARSER_SOL) sample_packet.bin
	@echo "\n--- UDP Packet ---"
	./$(PARSER_SOL) sample_udp_packet.bin
	@echo "\n--- Minimal Packet ---"
	./$(PARSER_SOL) minimal_packet.bin

# Run with sample packet
run: $(PARSER) $(SAMPLE_PACKETS)
	./$(PARSER) sample_packet.bin

# Compare your solution with reference
compare: $(PARSER) $(PARSER_SOL) $(SAMPLE_PACKETS)
	@echo "=== Your Solution Output ==="
	@./$(PARSER) sample_packet.bin > your_output.txt 2>&1
	@echo "=== Reference Solution Output ==="
	@./$(PARSER_SOL) sample_packet.bin > ref_output.txt 2>&1
	@echo "=== Differences (if any) ==="
	@diff -u ref_output.txt your_output.txt || echo "No differences!"

# Debug with debugger
debug: $(PARSER) $(SAMPLE_PACKETS)
	gdb ./$(PARSER)

# Clean up
clean:
	rm -f $(PARSER) $(PARSER_SOL) $(GENERATOR)
	rm -f *.o *.dSYM
	rm -f your_output.txt ref_output.txt
	@echo "Cleaned up build artifacts"

# Clean everything including generated samples
distclean: clean
	rm -f $(SAMPLE_PACKETS)
	@echo "Cleaned all generated files"

# Show structure sizes
sizes: $(PARSER_SOL)
	@echo "=== Struct Sizes ==="
	@$(CC) -E -dD -x c - < /dev/null > /dev/null 2>&1
	@$(CC) $(CFLAGS) -DCHECK_SIZES=1 -o /tmp/sizes $(SOLUTION_SRC)
	@echo "To verify struct sizes, modify source to include sizeof() checks"

# Help
help:
	@echo "Packet Header Parser - Makefile targets:"
	@echo ""
	@echo "  make              - Build parser, solution, and generator"
	@echo "  make run          - Build and run with sample packet"
	@echo "  make test         - Test reference solution"
	@echo "  make compare      - Compare your output with solution"
	@echo "  make debug        - Run under debugger"
	@echo "  make clean        - Remove compiled binaries"
	@echo "  make distclean    - Remove all generated files"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Getting started:"
	@echo "  1. Read LESSON_PLAN.md"
	@echo "  2. Read PROJECT_GUIDE.md"
	@echo "  3. Edit 02_starter.c"
	@echo "  4. make run"
	@echo "  5. make compare"

.PHONY: all test run compare debug clean distclean sizes help
