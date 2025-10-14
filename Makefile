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

build/compile_commands.json:
	echo -e '[{"directory": "/home/kimpors/project/cson",' 	\
		'"command": "/usr/bin/cc src/main.c src/token.c -o'  \
		'build/cson -I/home/kimpors/project/cson/include",'	\
		'"file": "src/main.c"}]' > build/compile_commands.json

clean:
	rm -f build/cson
