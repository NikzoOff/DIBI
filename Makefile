# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS =

# Installation paths
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
DESTDIR =

# Project files
TARGET = did-i-break-it
SOURCE = dibi.c

# Default target
all: $(TARGET)

# Build the binary
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

# Install the binary
install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
	@echo "Installed $(TARGET) to $(DESTDIR)$(BINDIR)/$(TARGET)"
	@echo "You can now run: did-i-break-it"

# Uninstall the binary
uninstall:
	rm -f $(BINDIR)/$(TARGET)
	@echo "Uninstalled $(TARGET) from $(BINDIR)/$(TARGET)"

# Clean build artifacts
clean:
	rm -f $(TARGET)
	@echo "Cleaned build artifacts"

# Rebuild from scratch
rebuild: clean all

# Help message
help:
	@echo "Available targets:"
	@echo "  all       - Build the binary (default)"
	@echo "  install   - Install the binary to $(BINDIR)"
	@echo "  uninstall - Remove the installed binary"
	@echo "  clean     - Remove build artifacts"
	@echo "  rebuild   - Clean and rebuild"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Variables:"
	@echo "  PREFIX    - Installation prefix (default: /usr/local)"
	@echo "  DESTDIR   - Staging directory for packaging"
	@echo "  CC        - Compiler (default: gcc)"
	@echo "  CFLAGS    - Compiler flags"

# Phony targets (not actual files)
.PHONY: all install uninstall clean rebuild help
