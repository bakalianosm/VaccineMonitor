# Path for
INCLUDE = include
MODULES = modules
SOURCE = src
INPUT = inputfile.txt
# Command line arguments
ARGS = 

# Compiler
CC = gcc 

# Compile options
CFLAGS = -Wall -g -I$(INCLUDE)
LDFLAGS = -lm

# .o files
OBJS = $(SOURCE)/vaccineMonitor.o $(MODULES)/linkedList.o  $(MODULES)/bloomFilter.o  $(MODULES)/skipList.o  $(MODULES)/generalFunctions.o 

# The executable program
EXEC = vaccineMonitor

# Run targets

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC) 

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind ./$(EXEC) $(ARGS)

run_no_args: $(EXEC)
	time ./$(EXEC) 

valgrind_no_args: $(EXEC)
	valgrind ./$(EXEC) 