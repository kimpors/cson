SHELL := /bin/bash

CC = gcc
SRC = src/main.c src/token.c src/test.c
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
	@$(TARGET) $< 2>&1 | diff  $(word 2, $?) - || \
		(echo 'Test $@ failed' && exit 1)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(DBFLAGS)

build/compile_commands.json:
	echo -e '[{"directory": "$(shell pwd)",' 	\
		'"command": "/usr/bin/cc src/main.c src/token.c -o'  \
		'build/cson -I$(shell pwd)/include",'	\
		'"file": "src/main.c"}]' > build/compile_commands.json

clean:
	rm -f build/test
