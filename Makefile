CC      := gcc
CFLAGS  := 	-Wall \
			-Iinclude \
			-Iconfig \
			-Ilib \
			-I/usr/include/SDL2 \
CC      := gcc
LDFLAGS := -lSDL2 -lSDL2_ttf
SRC     := app.c $(wildcard src/*.c) $(wildcard include/*.c) $(wildcard lib/queue/*.c)
OBJ     := $(SRC:.c=.o)
BIN     := app

.PHONY: all sim clean
.PHONY: all sim clean deps

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

sim: $(BIN)
	./$(BIN)

clean:
	rm -vf $(OBJ) $(BIN)


deps:
	$(CC) $(CFLAGS) -H -c src/main.c 2>&1 | tee deps.txt
