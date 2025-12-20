SHELL := /bin/bash

CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(addprefix build/, $(notdir $(addsuffix .o, $(basename $(wildcard src/*c)))))
TARGET = build/cson
STATIC_TARGET = $(TARGET).a
SHARED_TARGET = $(TARGET).so
CFLAGS += -Iinclude
MAKEFLAGS += --no-print-directory

.PHONY: static shared compile token-test parse-test clean

shared: $(SHARED_TARGET)
static: $(STATIC_TARGET)

$(STATIC_TARGET): $(OBJ)
	@echo "Building static library to $@"
	@ar -rcs $(STATIC_TARGET) $(OBJ)

$(SHARED_TARGET): $(SRC)
	@echo "Building shared library to $@"
	@gcc -shared -fPIC -Iinclude -o $@ $^

$(OBJ): $(SRC)
	@echo "Compiling object file to $@"
	@$(CC) -c $(CFLAGS) $(addprefix src/, $(notdir $(basename $@))).c -o $@

compile: build/compile_commands.json

test: token-test parse-test

token-test:
	@echo "token test"
	@cd test/token && $(MAKE)

parse-test:
	@echo "parse test"
	@cd test/parse && $(MAKE)

build/compile_commands.json:
	echo -e '[{"directory": "$(shell pwd)",' 	\
		'"command": "/usr/bin/cc src/main.c src/token.c -o'  \
		'build/cson -I$(shell pwd)/include",'	\
		'"file": "src/main.c"}]' > build/compile_commands.json

clean:
	@rm -f build/cson.*
