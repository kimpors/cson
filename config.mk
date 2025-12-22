SHELL := /bin/bash

SRC = $(wildcard src/*.c)
OBJ = $(addprefix $(BUILD)/, $(notdir $(addsuffix .o, $(basename $(wildcard src/*c)))))

BUILD = build
TARGET = $(BUILD)/cson
STATIC_TARGET = $(TARGET).a
SHARED_TARGET = $(TARGET).so

CC = gcc
DBFLAGS += -g
CFLAGS += -Iinclude
MAKEFLAGS += --no-print-directory
