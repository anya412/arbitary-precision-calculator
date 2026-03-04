# Makefile for Arbitrary Precision Calculator
# -----------------------------------------------
# Description:
#   - Collects all .c files from the current directory
#   - Compiles them into object files inside 'obj/' folder
#   - Links all object files to create final executable 'calc.out'
#   - Provides 'clean' target to remove build artifacts
# -----------------------------------------------

# Collect all source files
SRC := $(wildcard *.c)

# Directory to store object files
OBJDIR := obj

# Convert .c → obj/%.o
OBJ := $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

# Final executable target
calc.out: $(OBJ)
	gcc -o $@ $^

# Rule to build .o files inside obj/
# $< = source file, $@ = target object file
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	gcc -c $< -o $@

# Create obj/ folder if it does not exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean rule → removes object directory and executable
clean:
	rm -rf $(OBJDIR) calc.out