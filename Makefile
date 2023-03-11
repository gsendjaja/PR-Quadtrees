#makefile skeleton taken from workshop 3.8
# define C compiler & flags
CC = gcc
CFLAGS = -Wall -g


# define sets of header source files and object files
SRC = driver.c data.c array.c quad.c 
# OBJ is the same as SRC, just replace .c with .h
OBJ = $(SRC:.c=.o)
 
# define the executable name(s)
EXE1 = dict3
EXE2 = dict4


# the first target:
$(EXE1): $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE1) $(OBJ) $(LIB)

# the second target:
$(EXE2): $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ) $(LIB)

all: $(EXE1) $(EXE2)

driver.o: driver.c data.h array.h quad.h structures.h

data.o: data.c data.h structures.h

array.o: array.c array.h data.h quad.o quad.h structures.h

quad.o: data.h data.c structures.h

clean:
	rm -f $(OBJ) $(EXE1) $(EXE2)
