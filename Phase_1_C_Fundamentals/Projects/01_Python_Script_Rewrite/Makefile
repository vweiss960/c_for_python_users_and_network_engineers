# Makefile for Word Counter Project
# Simple build automation for the C version

CC = gcc
CFLAGS = -Wall -g -std=c99
PROGRAMS = word_counter

# Default target
all: $(PROGRAMS)

# Build the word counter
word_counter: 01_c_solution.c
	$(CC) $(CFLAGS) 01_c_solution.c -o word_counter

# Run with sample input
run: word_counter
	./word_counter sample_input.txt

# Run with Python version for comparison
python_run:
	python3 01_python_solution.py sample_input.txt

# Compare both versions
compare: word_counter python_run
	@echo "=== C Version ==="
	./word_counter sample_input.txt
	@echo ""
	@echo "=== Python Version ==="
	python3 01_python_solution.py sample_input.txt

# Clean up compiled files
clean:
	rm -f $(PROGRAMS) *.o

# Debug target (run with gdb)
debug: word_counter
	gdb ./word_counter

# Help
help:
	@echo "Makefile targets:"
	@echo "  make all      - Build the C program (default)"
	@echo "  make run      - Build and run with sample_input.txt"
	@echo "  make python_run - Run the Python version"
	@echo "  make compare  - Run both C and Python versions"
	@echo "  make clean    - Remove compiled files"
	@echo "  make debug    - Run under gdb debugger"
	@echo "  make help     - Show this message"

.PHONY: all run python_run compare clean debug help
