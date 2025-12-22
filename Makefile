SHELL := /bin/bash

CC = gcc
BUILD = build
SRC = $(wildcard src/*.c)
OBJ = $(addprefix $(BUILD)/, $(notdir $(addsuffix .o, $(basename $(wildcard src/*c)))))
TARGET = $(BUILD)/cson
STATIC_TARGET = $(TARGET).a
SHARED_TARGET = $(TARGET).so
CFLAGS += -Iinclude
MAKEFLAGS += --no-print-directory

.PHONY: static shared compile token-test parse-test clean

shared: $(BUILD) $(SHARED_TARGET)
static: $(BUILD) $(STATIC_TARGET)

$(STATIC_TARGET): $(OBJ)
	@echo "Building static library to $@"
	@ar -rcs $(STATIC_TARGET) $(OBJ)

$(SHARED_TARGET): $(SRC)
	@echo "Building shared library to $@"
	@gcc -shared -fPIC -Iinclude -o $@ $^

$(OBJ): $(SRC)
	@echo "Compiling object file to $@"
	@$(CC) -c $(CFLAGS) $(addprefix src/, $(notdir $(basename $@))).c -o $@

$(BUILD):
	@echo "Creating build directory"
	@if [ ! -d "./$(BUILD)" ]; then \
		mkdir $(BUILD); \
	fi

compile: $(BUILD)/compile_commands.json

test: token-test parse-test

token-test:
	@echo "token test"
	@cd test/token && $(MAKE)

parse-test:
	@echo "parse test"
	@cd test/parse && $(MAKE)

$(BUILD)/compile_commands.json: $(BUILD)
	echo -e '[{"directory": "$(shell pwd)",' 	\
		'"command": "/usr/bin/cc src/main.c src/token.c -o'  \
		'$(BUILD)/cson -I$(shell pwd)/include",'	\
		'"file": "src/main.c"}]' > $(BUILD)/compile_commands.json

clean:
	@rm -f $(BUILD)/cson.*
