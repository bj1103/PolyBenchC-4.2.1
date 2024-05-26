# Compiler
CC = gcc

# Compiler flags
FLAGS = -O3 -I utilities

# Directories
SRCDIR = .
BINDIR = test

# Create bin directory if it doesn't exist
$(shell mkdir -p $(BINDIR))

# Source files
SRC = $(wildcard $(SRCDIR)/datamining/*/*.c) \
      $(wildcard $(SRCDIR)/linear-algebra/*/*/*.c) \
      $(wildcard $(SRCDIR)/medley/*/*.c) \
      $(wildcard $(SRCDIR)/stencils/*/*.c) \


# Executable files
EXE = $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%, $(SRC))
TEST = test

# Default target
all: $(EXE)

# Rule to build each executable with specific include paths
$(BINDIR)/%: $(SRCDIR)/%.c
	$(CC) $(FLAGS) -I $(dir $<) utilities/polybench.c $< -DPOLYBENCH_TIME -lm -o $(BINDIR)/$(notdir $@)

run: $(TEST)
	gcc test.c -o exp
	./exp

# Clean target
clean:
	rm -f $(BINDIR)/*
	rm exp
