include $(CONFIG_PATH)

SRC = src/main.c src/parse.c src/token.c src/test.c
TARGET := $(BUILD)/test

all-tests := $(addsuffix .test, $(basename $(wildcard example/*.in)))

.PHONY : all test %.test clean

all: $(BUILD) $(TARGET) $(all-tests)
	@echo "Success, all test passed."

compile: $(BUILD) $(BUILD)/compile_commands.json

example/%.test: example/%.in example/%.cmp
	@$(TARGET) $< 2>&1 | diff  $(word 2, $?) - || \
		(echo 'Test $@ failed' && exit 1)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(DBFLAGS)

$(BUILD):
	@echo "Creating build directory"
	@if [ ! -d "./$(BUILD)" ]; then \
		mkdir $(BUILD); \
	fi

$(BUILD)/compile_commands.json:
	echo -e '[{"directory": "$(shell pwd)",' 	\
		'"command": "/usr/bin/cc src/main.c src/token.c -o'  \
		'$(BUILD)/cson -I$(shell pwd)/include",'	\
		'"file": "src/main.c"}]' > $(BUILD)/compile_commands.json
clean:
	rm -f $(BUILD)/test
