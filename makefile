# Nome dell'eseguibile
TARGET = memos.o

# File sorgente
SRC = src/main.c src/drawer.c src/gameScreen.c src/menuScreen.c

# Compiler
CC = gcc

# Flags del compilatore
CFLAGS = -Wall -std=c11 -Iinclude `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf

# Regola principale
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Pulizia
clean:
	rm -f $(TARGET)
