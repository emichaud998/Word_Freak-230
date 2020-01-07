SRC = readfile.c
OBJ = readfile.o
PROG = wordfreak

CC=gcc
CFLAGS = -g

$(PROG) : $(OBJ)
	$(CC) $(OBJ) -o $(PROG) $(CFLAGS)

$(OBJ): $(SRC)
	$(CC) -c -o $(OBJ) $(SRC) $(CFLAGS)

clean:
	rm -f $(OBJ) $(PROG)

