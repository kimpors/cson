SHELL := /bin/bash

.PHONY: clean compile
.DEFAULT_GOAL = all

SRC = $(wildcard src/*.c)
OBJ := $(addprefix $(BUILD)/, $(notdir $(addsuffix .o, $(basename $(wildcard src/*c)))))

TESTS = token-to-json parse-to-json token parse 
ABS_SRC := $(subst /config.mk,,$(abspath $(lastword $(MAKEFILE_LIST))))/src/*.c
ABS_INC := $(subst /config.mk,,$(abspath $(lastword $(MAKEFILE_LIST))))/include

BUILD := build
BUILD_TARGET := $(BUILD)/cson
BUILD_STATIC_TARGET := $(BUILD_TARGET).a
BUILD_SHARED_TARGET := $(BUILD_TARGET).so

TARGET := cson
STATIC_TARGET := $(TARGET).a
SHARED_TARGET := $(TARGET).so

CC := gcc
DBFLAGS += -gdwarf-2 -g3
CFLAGS += -Iinclude -lm
MAKEFLAGS += --no-print-directory

# install path
DESTDIR := /usr/local

$(BUILD):
	@echo "Creating build directory"
	@if [ ! -d "./$(BUILD)" ]; then \
		mkdir $(BUILD); \
	fi

$(BUILD)/compile_commands.json: $(BUILD)
	echo -e '[{"directory": "$(shell pwd)",' 	\
		'"command": "/usr/bin/cc $(ABS_SRC) src/* -o'  \
		'$(BUILD)/cson -I$(ABS_INC) -I./include",'	\
		'"file": "src/main.c"}]' > $(BUILD)/compile_commands.json

compile: $(BUILD)/compile_commands.json

clean:
	@rm -rf $(BUILD)
