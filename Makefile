CC       := gcc
CFLAGS   := -Wall -g
INCFLAGS := -Iinclude \
            -Iconfig \
            -Ilib \
            -I/usr/include/SDL2

LDFLAGS  := -lSDL2 -lSDL2_ttf -lpthread

SRC      := app.c \
            $(wildcard src/*.c) \
            $(wildcard lib/queue/*.c)

OBJ      := $(SRC:.c=.o)
BIN      := app

.PHONY: all sim clean deps

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
	$(CC) $(CFLAGS) $(INCFLAGS) -H -c src/main.c 2>&1 | tee deps.txt
