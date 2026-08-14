CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c11 -Iinclude

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = Equilibrium

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@/bin/sh -c "./$(TARGET)"

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean run 
