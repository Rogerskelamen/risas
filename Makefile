PROG = risas
SRC  = $(PROG).c
OBJ  = $(SRC:.c  = .o)

CC = cc
CFLAGS = -Wall -Werror
LDFLAGS =

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) $(LDFLAGS) $< -o $@

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	-rm $(OBJ)

.PHONY: all clean