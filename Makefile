CC = gcc
SRC = src/main.c src/token.c src/parse.c
OBJ = $(SRC:.c=.o)
TARGET = build/cson
CFLAGS += -Iinclude
DBFLAGS += -g
MAKEFLAGS += --no-print-directory

.PHONY: all compile token-tests clean

all: $(TARGET)

compile: build/compile_commands.json

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(DBFLAGS)

token-test:
	@echo "token test"
	@cd test/token && $(MAKE)

build/compile_commands.json:
	echo -e '[{"directory": "/home/kimpors/project/cson",' 	\
		'"command": "/usr/bin/cc src/main.c src/token.c -o'  \
		'build/cson -I/home/kimpors/project/cson/include",'	\
		'"file": "src/main.c"}]' > build/compile_commands.json

clean:
	rm -f build/cson
