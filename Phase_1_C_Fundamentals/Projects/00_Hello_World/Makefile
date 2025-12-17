# Makefile for Hello World Project
# Simplest possible C program to verify your setup

CC = gcc
CFLAGS = -Wall -g

# Build the program
hello: hello.c
	$(CC) $(CFLAGS) hello.c -o hello

# Run the program
run: hello
	./hello

# Clean up
clean:
	rm -f hello hello.o

# Help
help:
	@echo "Hello World - Setup Verification"
	@echo "================================="
	@echo ""
	@echo "Available commands:"
	@echo "  make           - Compile the program"
	@echo "  make run       - Compile and run (prints 'Hello, World!')"
	@echo "  make clean     - Remove compiled files"
	@echo "  make help      - Show this message"
	@echo ""
	@echo "If 'make run' prints 'Hello, World!', your C setup is working!"

.PHONY: run clean help
