CC=gcc
CFLAGS=-I. -Werror -Wextra -Wall -Wno-unused-parameter -g
LDFLAGS = -lpthread
BIN = race race2

all: $(BIN)

race: race.c
	@$(CC) -g -o race race.c $(CFLAGS) $(LDFLAGS)

race2: race2.c
	@$(CC) -g -o race2 race2.c $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	@rm -f *.o $(BIN)
