include config.mk

.PHONY: install static shared compile token-test parse-test clean

all: shared
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
	@$(MAKE) -C test/token CONFIG_PATH=$(shell pwd)/config.mk

parse-test:
	@echo "parse test"
	@$(MAKE) -C test/parse CONFIG_PATH=$(shell pwd)/config.mk

$(BUILD)/compile_commands.json: $(BUILD)
	echo -e '[{"directory": "$(shell pwd)",' 	\
		'"command": "/usr/bin/cc src/main.c src/token.c -o'  \
		'$(BUILD)/cson -I$(shell pwd)/include",'	\
		'"file": "src/main.c"}]' > $(BUILD)/compile_commands.json

clean:
	@rm -f $(BUILD)/cson.*
