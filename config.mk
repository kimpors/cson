SHELL := /bin/bash

SRC = $(wildcard src/*.c)
OBJ = $(addprefix $(BUILD)/, $(notdir $(addsuffix .o, $(basename $(wildcard src/*c)))))

BUILD = build
BUILD_TARGET = $(BUILD)/cson
BUILD_STATIC_TARGET = $(BUILD_TARGET).a
BUILD_SHARED_TARGET = $(BUILD_TARGET).so

TARGET = cson
STATIC_TARGET = $(TARGET).a
SHARED_TARGET = $(TARGET).so

CC = gcc
DBFLAGS += -gdwarf-2 -g3
CFLAGS += -Iinclude -lm
MAKEFLAGS += --no-print-directory

# install path
DESTDIR = /usr/local
