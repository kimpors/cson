CC = gcc
SRC = src/main.c src/token.c src/parse.c
OBJ = $(SRC:.c=.o)
TARGET = build/cson
CFLAGS += -Iinclude

all: $(TARGET)
run: $(TARGET) 
	./build/cson

compile: build/compile_commands.json

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

clean:
	rm -f build/cson
