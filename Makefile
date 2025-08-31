CC=gcc
CFLAGS=-Wall -Iinclude `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs` -lSDL2_ttf
SRC=app.c $(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: app

app: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f app $(OBJ)
