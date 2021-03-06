# Path for
INCLUDE = include
MODULES = modules
SOURCE = src
MISC = misc
# Command line arguments
ARGS = -c $(MISC)/inputfile.txt -b 100 


# Compiler
CC = gcc 

# Compile options
CFLAGS = -Wall -g -I$(INCLUDE)
LDFLAGS = -lm

# .o files
OBJS = $(SOURCE)/vaccineMonitor.o $(MODULES)/linkedList.o  $(MODULES)/bloomFilter.o  $(MODULES)/skipList.o  $(MODULES)/generalFunctions.o 
OBJS2= $(SOURCE)/ADTMap_test.o $(MODULES)/map.o $(MODULES)/linkedList.o
# The executable program
EXEC = vaccineMonitor
EXEC2 = ADTMap_test
# Run targets

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

$(EXEC2): $(OBJS2)
	$(CC) $(OBJS2) -o $(EXEC2) $(LDFLAGS)
clean:
	rm -f $(OBJS) $(EXEC) $(OBJS2) $(EXEC2)

run: $(EXEC2)
	./$(EXEC2) $(ARGS)

valgrind: $(EXEC)
	valgrind ./$(EXEC) $(ARGS)

run_no_args: $(EXEC)
	time ./$(EXEC) 

valgrind_no_args: $(EXEC)
	valgrind ./$(EXEC) 