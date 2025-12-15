SHELL := /bin/bash

CC = gcc
SRC = src/main.c src/token.c src/test.c
OBJ = $(SRC:.c=.o)
TARGET := build/test
CFLAGS += -Iinclude
DBFLAGS += -g

all-tests := $(addsuffix .test, $(basename $(wildcard example/*.in)))

.PHONY : all test %.test clean

all: $(TARGET) test
	@echo "Success, all test passed."

compile: build/compile_commands.json

test: $(all-tests)

example/%.test: example/%.in example/%.cmp
	@$(TARGET) $< 2>&1 | diff -q $(word 2, $?) - > /dev/null || \
		(echo 'Test $@ failed' && exit 1)


$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(DBFLAGS)


build/compile_commands.json:
	echo -e '[{"directory": "/home/kimpors/project/cson/test/token",' 	\
		'"command": "/usr/bin/cc src/main.c src/token.c -o'  \
		'build/test -I/home/kimpors/project/cson/test/token/include",'	\
		'"file": "src/main.c"}]' > build/compile_commands.json
clean:
	rm -f build/test
