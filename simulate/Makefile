# simulate/Makefile

CC      := gcc
CFLAGS  := -Wall -Wextra -g \
           -Isrc -Iinclude \
           -I../include -I../config \
           -I../lib/log -I../lib/queue -I../lib/interrupt \
           -I../app

TARGET  := simulate.out

SRC := src/main.c \
       ../app.c \
       ../lib/log/log.c \
       ../lib/queue/queue.c \
       ../lib/interrupt/sim_interrupt.c

OBJ := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
