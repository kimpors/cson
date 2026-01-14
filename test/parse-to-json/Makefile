include ../../config.mk

SRC = src/main.c $(ABS_SRC)
CFLAGS += -I$(ABS_INC)
TARGET := $(BUILD)/test

all-tests := $(addsuffix .test, $(basename $(wildcard example/*.in)))

.PHONY : all test %.test

all: $(BUILD) $(TARGET) $(all-tests)
	@echo "Success, all test passed."

example/%.test: example/%.in example/%.cmp
	@$(TARGET) $< 2>&1 | diff $(word 2, $?) - || \
		(echo 'Test $@ failed' && exit 1)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(DBFLAGS)
