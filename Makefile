include config.mk

.PHONY: install-shared install-static install-include \
	static shared compile token-test parse-test clean

all: shared
test: token-test parse-test parse-to-json-test
shared: $(BUILD) $(BUILD_SHARED_TARGET)
static: $(BUILD) $(BUILD_STATIC_TARGET)

install-shared: shared install-include
	mkdir -p ${DESTDIR}/lib
	cp -f $(BUILD_SHARED_TARGET) ${DESTDIR}/lib
	chmod 755 ${DESTDIR}/lib/$(SHARED_TARGET)

install-static: static install-include
	mkdir -p ${DESTDIR}/lib
	cp -f $(BUILD_STATIC_TARGET) ${DESTDIR}/lib
	chmod 755 ${DESTDIR}/lib/$(STATIC_TARGET)

install-include:
	mkdir -p ${DESTDIR}/include/$(TARGET)
	cp -f include/* ${DESTDIR}/include/$(TARGET)
	chmod 755 ${DESTDIR}/include/$(TARGET)

uninstall:
	rm -f ${DESTDIR}/lib/$(TARGET).*
	rm -rf $(DESTDIR)/include/$(TARGET)

$(BUILD_STATIC_TARGET): $(OBJ)
	@echo "Building static library to $@"
	@ar -rcs $(BUILD_STATIC_TARGET) $(OBJ)

$(BUILD_SHARED_TARGET): $(SRC)
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

token-test:
	@echo "token test"
	@$(MAKE) -C test/token CONFIG_PATH=$(shell pwd)/config.mk

parse-test:
	@echo "parse test"
	@$(MAKE) -C test/parse CONFIG_PATH=$(shell pwd)/config.mk

parse-to-json-test:
	@echo "parse test"
	@$(MAKE) -C test/parse-to-json CONFIG_PATH=$(shell pwd)/config.mk

$(BUILD)/compile_commands.json: $(BUILD)
	echo -e '[{"directory": "$(shell pwd)",' 	\
		'"command": "/usr/bin/cc src/main.c src/token.c -o'  \
		'$(BUILD)/cson -I$(shell pwd)/include",'	\
		'"file": "src/main.c"}]' > $(BUILD)/compile_commands.json

clean:
	@rm -f $(BUILD)/cson.*
