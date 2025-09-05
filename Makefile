CC       := gcc
CFLAGS   := -Wall -g
INCFLAGS := -Iinclude \
            -Iconfig \
            -Ilib \
            -Isimulate/include \
            -I/usr/include/SDL2

LDFLAGS  := -lSDL2 -lSDL2_ttf -lpthread

SRC      := app.c \
            $(wildcard simulate/src/*.c) \
            $(wildcard lib/queue/*.c) \
            $(wildcard lib/log/*.c) \
            $(wildcard lib/interrupt/*.c)

OBJ      := $(SRC:.c=.o)
BIN      := app

.PHONY: all sim clean deps leak_check

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

sim: $(BIN)
	./$(BIN)

clean:
	rm -vf $(OBJ) $(BIN)

deps:
	$(CC) $(CFLAGS) $(INCFLAGS) -H -c simulate/src/main.c 2>&1 | tee deps.txt

leak_check:
	valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all \
		--log-file=valgrind.%p.log ./$(BIN)
