PROG  = risas
DEST  = /usr/local/bin
BUILD_DIR = build

SRCS = $(shell find src -name "*.c")
OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SRCS))))

INCFLAGS = -Iinclude

CFLAGS += -Wall -Werror
CFLAGS += -MMD -O2
CFLAGS += $(INCFLAGS)
LDFLAGS =

all: $(PROG)

$(PROG): $(OBJS)
	@echo + LD "->" $@
	@$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@) && echo + CC $<
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm -f $(PROG)
	-rm -rf $(BUILD_DIR)

install: all
	@echo $(PROG) "->" $(DEST)
	@cp $(PROG) $(DEST)

uninstall:
	rm $(DEST)/$(PROG)

count:
	@find . -type f -name "*.[ch]" | xargs wc -l

.PHONY: all clean count install uninstall
