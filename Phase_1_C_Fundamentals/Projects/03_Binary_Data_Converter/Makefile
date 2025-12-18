# Makefile for Binary Data Converter Project
# Usage: make [target]

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
LDFLAGS = -lm

# Targets
.PHONY: all clean run test compare help converter_solution test_vectors debug

# Default target - build everything
all: converter converter_solution generate_test_data

# ============================================================================
# BUILD TARGETS
# ============================================================================

# Build the solution (reference implementation)
converter_solution: 03_c_solution.c
	$(CC) $(CFLAGS) -o converter_solution 03_c_solution.c

# Build the starter template (for learner to fill in)
converter: 03_starter.c
	$(CC) $(CFLAGS) -o converter 03_starter.c

# Build test data generator
generate_test_data: generate_test_data.c
	$(CC) $(CFLAGS) -o generate_test_data generate_test_data.c

# ============================================================================
# RUN TARGETS
# ============================================================================

# Run the solution with sample input
run: converter_solution
	@echo "=== Running reference solution ==="
	./converter_solution 255 all
	@echo ""
	./converter_solution 192.168.1.1 ip
	@echo ""
	./converter_solution 0xDEADBEEF swap
	@echo ""
	./converter_solution detect-endian

# Run your version with same inputs
run_your_version: converter
	@echo "=== Running your solution ==="
	./converter 255 all
	@echo ""
	./converter 192.168.1.1 ip
	@echo ""
	./converter 0xDEADBEEF swap
	@echo ""
	./converter detect-endian

# ============================================================================
# TEST TARGETS
# ============================================================================

# Generate test vectors
test_vectors: generate_test_data
	@echo "Generating test vectors..."
	./generate_test_data > test_vectors.txt
	@echo "Test vectors written to test_vectors.txt"
	@echo ""
	@echo "Sample test vectors:"
	@head -50 test_vectors.txt

# Run basic tests on solution
test: converter_solution
	@echo "=== Testing Binary Conversions ==="
	@echo ""
	@echo "Test 1: Zero"
	@./converter_solution 0 all
	@echo ""
	@echo "Test 2: 255 (max byte)"
	@./converter_solution 255 all
	@echo ""
	@echo "Test 3: 256 (powers of 2)"
	@./converter_solution 256 all
	@echo ""
	@echo "Test 4: 42 (common value)"
	@./converter_solution 42 all
	@echo ""
	@echo "Test 5: IP address"
	@./converter_solution 192.168.1.1 ip
	@echo ""
	@echo "Test 6: System endianness"
	@./converter_solution detect-endian
	@echo ""
	@echo "Test 7: Byte swap"
	@./converter_solution 0xDEADBEEF swap

# Extended test suite
test_extended: converter_solution
	@echo "=== Extended Test Suite ==="
	@echo ""
	@echo "Testing various inputs..."
	@for val in 0 1 2 4 8 127 128 255 256 1024 65535 0xFFFFFFFF; do \
		echo "Testing $$val:"; \
		./converter_solution $$val all 2>/dev/null | head -5; \
		echo ""; \
	done

# ============================================================================
# COMPARISON AND VALIDATION
# ============================================================================

# Compare your output with reference solution
compare: converter converter_solution
	@echo "=== Comparing Your Solution with Reference ==="
	@echo ""
	@echo "Test: 255 all"
	@echo "Your version:"
	@./converter 255 all 2>/dev/null || echo "ERROR: Your converter failed"
	@echo ""
	@echo "Reference version:"
	@./converter_solution 255 all
	@echo ""
	@echo "=== Manual Comparison Instructions ==="
	@echo "1. Check that both outputs match exactly"
	@echo "2. The order and formatting should be identical"
	@echo "3. Test other values: make test_compare VAL=42"

# Test with custom value
test_compare: converter converter_solution
	@echo "=== Comparing with value: $(VAL) ==="
	@echo ""
	@echo "Your version:"
	@./converter $(VAL) all 2>/dev/null || echo "ERROR"
	@echo ""
	@echo "Reference version:"
	@./converter_solution $(VAL) all

# ============================================================================
# DEBUGGING TARGETS
# ============================================================================

# Run under debugger
debug: converter
	gdb ./converter

# Run with verbose output
debug_solution: converter_solution
	./converter_solution 255 all
	@echo ""
	./converter_solution 192.168.1.1 ip
	@echo ""
	./converter_solution detect-endian

# Analyze with address sanitizer (detect memory errors)
asan: converter
	$(CC) $(CFLAGS) -fsanitize=address,undefined -o converter_asan 03_starter.c
	./converter_asan 255 all
	rm -f converter_asan

# ============================================================================
# UTILITY TARGETS
# ============================================================================

# Show usage information
help:
	@echo "Binary Data Converter - Makefile targets"
	@echo ""
	@echo "Build targets:"
	@echo "  make all               - Build everything (default)"
	@echo "  make converter         - Build your version (from 03_starter.c)"
	@echo "  make converter_solution - Build reference solution"
	@echo ""
	@echo "Run targets:"
	@echo "  make run               - Run reference solution with sample inputs"
	@echo "  make run_your_version  - Run your version with same inputs"
	@echo ""
	@echo "Test targets:"
	@echo "  make test              - Run basic test suite"
	@echo "  make test_extended     - Run extended test suite"
	@echo "  make test_vectors      - Generate test vectors"
	@echo "  make compare           - Compare your output with reference"
	@echo "  make test_compare VAL=42 - Test specific value"
	@echo ""
	@echo "Debug targets:"
	@echo "  make debug             - Run your version under gdb debugger"
	@echo "  make debug_solution    - Test reference solution"
	@echo "  make asan              - Check for memory errors with AddressSanitizer"
	@echo ""
	@echo "Cleanup:"
	@echo "  make clean             - Remove compiled binaries"
	@echo "  make clean_test        - Remove test output files"
	@echo ""
	@echo "Documentation:"
	@echo "  make help              - Show this help message"
	@echo ""
	@echo "Quick start:"
	@echo "  1. make converter          # Build from your template"
	@echo "  2. make run                # See what output should look like"
	@echo "  3. vim 03_starter.c        # Edit your solution"
	@echo "  4. make                    # Rebuild"
	@echo "  5. ./converter 255 all     # Test your version"
	@echo "  6. make compare            # Compare with reference"

# Show test categories
categories:
	@echo "Test Categories Available:"
	@echo ""
	@echo "1. Base Conversions:"
	@echo "   - Zero and powers of two"
	@echo "   - Boundary values (255, 256, 65535, 65536, etc.)"
	@echo "   - Common test values (42, 73, 127, 137, 192, 255)"
	@echo ""
	@echo "2. Bit Patterns:"
	@echo "   - All zeros (0x00000000)"
	@echo "   - All ones (0xFFFFFFFF)"
	@echo "   - Alternating patterns (0xAAAAAAAA, 0x55555555)"
	@echo "   - Single bit patterns"
	@echo ""
	@echo "3. Network Data:"
	@echo "   - IP addresses (127.0.0.1, 192.168.1.1, 10.0.0.1)"
	@echo "   - Port numbers"
	@echo "   - Byte order conversions"
	@echo ""
	@echo "4. Bit Fields:"
	@echo "   - Extract specific bits"
	@echo "   - Set field values"
	@echo "   - Complex field operations"
	@echo ""
	@echo "5. Round-trip Validation:"
	@echo "   - Convert A → B → A should equal A"
	@echo "   - Decimal → Binary → Decimal"
	@echo "   - Hex → Decimal → Hex"

# ============================================================================
# CLEANUP TARGETS
# ============================================================================

# Remove all compiled binaries
clean:
	@echo "Cleaning up compiled files..."
	rm -f converter
	rm -f converter_solution
	rm -f converter_asan
	rm -f generate_test_data
	rm -f *.o
	@echo "Clean complete"

# Remove test output files
clean_test:
	@echo "Removing test output files..."
	rm -f test_vectors.txt
	rm -f test_output.txt
	@echo "Test files removed"

# Clean everything
clean_all: clean clean_test
	@echo "All files cleaned"

# ============================================================================
# DEVELOPMENT WORKFLOW TARGETS
# ============================================================================

# Quick build and test workflow
quick: converter
	@./converter 255 all
	@echo ""
	@./converter_solution 255 all

# Full validation workflow
validate: all test compare
	@echo "=== Validation Complete ==="

# Prepare for submission
prepare: clean_all all test
	@echo "=== Ready for submission ==="
	@echo "Files to submit:"
	@echo "  - 03_starter.c (your solution)"
	@ls -la 03_starter.c 2>/dev/null || echo "  - 03_starter.c not found!"

# ============================================================================
# EXAMPLE USAGE DOCUMENTATION
# ============================================================================

examples:
	@echo ""
	@echo "EXAMPLE USAGE:"
	@echo ""
	@echo "Base conversions:"
	@echo "  ./converter 255 all"
	@echo "  ./converter 0xFF all"
	@echo "  ./converter 0b11111111 all"
	@echo "  ./converter 0377 all"
	@echo ""
	@echo "Single format:"
	@echo "  ./converter 42 binary"
	@echo "  ./converter 42 hex"
	@echo "  ./converter 42 octal"
	@echo ""
	@echo "IP addresses:"
	@echo "  ./converter 192.168.1.1 ip"
	@echo "  ./converter 127.0.0.1 ip"
	@echo ""
	@echo "System information:"
	@echo "  ./converter detect-endian"
	@echo ""
	@echo "Byte operations:"
	@echo "  ./converter 0x12345678 swap"
	@echo "  ./converter 0xDEADBEEF endian"
	@echo ""

# ============================================================================
# PHONY TARGETS (don't represent files)
# ============================================================================

.PHONY: all clean run test compare help converter_solution test_vectors debug examples quick validate prepare categories

# ============================================================================
# DEFAULT ACTION
# ============================================================================

# Print target list if make is run without arguments
.DEFAULT_GOAL := help

# ============================================================================
# NOTES
# ============================================================================

# Tips for using this Makefile:
#
# 1. Build and test:
#    make converter          # Compile your solution
#    ./converter 255 all     # Run it manually
#    make run                # See reference output
#
# 2. Compare outputs:
#    make compare            # Side-by-side comparison
#    make test_compare VAL=192.168.1.1  # Test specific value
#
# 3. Debug problems:
#    make debug              # Run in gdb
#    make asan               # Check for memory errors
#
# 4. Generate reference:
#    make test_vectors       # Create test_vectors.txt
#
# 5. Cleanup:
#    make clean              # Remove binaries
#    make clean_all          # Remove everything
