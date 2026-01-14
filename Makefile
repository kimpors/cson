include config.mk

.PHONY: all test install-shared install-static install-include \
	static shared 

all: shared
test:
	@for test in $(if $(args),$(args),$(TESTS)) ; do \
		echo "test $$test" ; \
		$(MAKE) -C test/$$test ; \
	done

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
