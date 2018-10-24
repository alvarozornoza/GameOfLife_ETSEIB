CC = gcc
CFLAGS = -Wall -I/usr/include/ -I.
LDFLAGS = -lncurses
DEPS = conway.h
OBJ = main.o init.o
PROF = -g -pg

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(PROF)

conway: $(OBJ)
	$(CC)  -o $@ $^  $(LDFLAGS) $(CFLAGS) $(PROF)

clean:
	rm -f $(OBJ) conway
