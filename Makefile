CC     := gcc
CFLAGS := -Wall -Wpedantic -Wextra -Werror -Ofast -DNDEBUG
RM     := rm -rf

PROGS = main

.PHONY: all

all: $(PROGS)

main: main.cpp
	@echo Generating: $@
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(PROGS)
