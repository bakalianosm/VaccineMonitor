# Path for

INCLUDE = include
MODULES = modules
SOURCE = src
MISC = misc
# Command line arguments
ARGS = -c $(MISC)/inputfile.txt -b 1000


# Compiler
CC = gcc 

# Compile options
CFLAGS = -Wall -g -I$(INCLUDE)
LDFLAGS = -lm

# .o files
OBJS = $(SOURCE)/vaccineMonitor.o $(MODULES)/linkedList.o  $(MODULES)/bloomFilter.o  $(MODULES)/skipList.o  $(MODULES)/generalFunctions.o $(MODULES)/map.o  $(MODULES)/dates.o $(MODULES)/userInput.o $(MODULES)/queryFunctions.o

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

 
