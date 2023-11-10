PROG = risas
SRC  = $(wildcard *.c)
OBJ  = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Werror
LDFLAGS =

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	-rm -f $(OBJ) $(PROG)

count:
	@find . -type f -name "*.[ch]" | xargs wc -l

.PHONY: all clean
