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
OBJS = $(SOURCE)/vaccineMonitor.o $(MODULES)/linkedList.o  $(MODULES)/bloomFilter.o  $(MODULES)/skipList.o  $(MODULES)/generalFunctions.o $(MODULES)/map.o  $(MODULES)/monitorChoices.o  
OBJS2= $(SOURCE)/ADTMap_test.o $(MODULES)/map.o $(MODULES)/linkedList.o
OBJS3= $(SOURCE)/skip.o $(MODULES)/linkedList.o $(MODULES)/skipList.o  $(MODULES)/bloomFilter.o $(MODULES)/map.o $(MODULES)/generalFunctions.o
# The executable program
EXEC = vaccineMonitor
EXEC2 = ADTMap_test
EXEC3 = skip
# Run targets

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

$(EXEC2): $(OBJS2)
	$(CC) $(OBJS2) -o $(EXEC2) $(LDFLAGS)

$(EXEC3): $(OBJS3)
	$(CC) $(OBJS3) -o $(EXEC3) $(LDFLAGS)	
clean:
	rm -f $(OBJS) $(EXEC) $(OBJS2) $(EXEC2) $(OBJS3) $(EXEC3)

run: $(EXEC)
	./$(EXEC) $(ARGS)

sk: $(EXEC3)
	./$(EXEC3) 

valgrind: $(EXEC)
	valgrind ./$(EXEC) $(ARGS)

run_no_args: $(EXEC)
	time ./$(EXEC) 

valgrind_no_args: $(EXEC)
	valgrind ./$(EXEC) 