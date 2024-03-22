PROG = risas
DEST = /usr/local/bin
SRC  = $(wildcard *.c)
OBJ  = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Werror
LDFLAGS =

all: $(PROG)

$(PROG): $(OBJ)
	@echo + LD "->" $@
	@$(CC) $(LDFLAGS) $^ -o $@

.c.o:
	@echo + CC $<
	@$(CC) -c $(CFLAGS) $<

clean:
	-rm -f $(OBJ) $(PROG)

install: all
	@echo $(PROG) "->" $(DEST)
	@cp $(PROG) $(DEST)

uninstall:
	rm $(DEST)/$(PROG)

count:
	@find . -type f -name "*.[ch]" | xargs wc -l

.PHONY: all clean count
