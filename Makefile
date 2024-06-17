VENDOR = extra/vendor
SKETCHES = examples/*
BIN = bin
SRC = src

ACC = arduino-cli
ACC_FLAGS = --library $(SRC) --library $(VENDOR)/Servo

PORT = /dev/ttyUSB0
CORE = arduino:avr
BOARD = arduino:avr:uno
BAUD = 115200

CC = clang++
CC_FLAGS = -Wall -I$(VENDOR)/googletest/googletest/include -I$(VENDOR)/googletest/googletest
LD_FLAGS = -lpthread

GTEST = $(VENDOR)/googletest
GTEST_DIR = extra/gtest
GTEST_INIT = $(GTEST_DIR)/main.cpp
GTEST_UNITS = $(wildcard $(GTEST_DIR)/test_*.cpp)
GTEST_SRCS = $(wildcard $(SRC)/*.cpp)
GTEST_BIN = $(BIN)/gtest
GTEST_LIBS = $(GTEST)/build/lib/libgtest.a $(GTEST)/build/lib/libgtest_main.a

DOXYGEN = $(VENDOR)/doxygen
DOXYGEN_BIN = $(BIN)/doxygen

.PHONY: default
default: compile test

.PHONY: deps
deps: deps/gtest deps/doxygen

.PHONY: deps/gtest
deps/gtest: deps/vendor
	mkdir -p $(GTEST)/build && cd $(GTEST)/build && cmake .. && make

.PHONY: deps/doxygen
deps/doxygen: deps/vendor
	mkdir -p $(DOXYGEN)/build && cd $(DOXYGEN)/build && cmake .. && make
	mkdir -vp $(BIN)
	mv -v $(DOXYGEN)/build/bin/doxygen $(DOXYGEN_BIN)

.PHONY: deps/vendor
deps/vendor:
	git submodule update --init --recursive

.PHONY: compile
compile: $(SKETCHES)
	for i in $^; do \
		$(ACC) compile --verbose $(ACC_FLAGS) --port $(PORT) --fqbn $(BOARD) $$i; \
	done;

.PHONY: upload
upload: $(SKETCHES) # Will execute only for the first element on the list!
	for i in $^; do \
		$(ACC) compile --verbose $(ACC_FLAGS) --port $(PORT) --fqbn $(BOARD) --upload $$i; \
		break; \
	done;

.PHONY: serial
serial:
	$(ACC) monitor --port $(PORT) --config "baudrate=$(BAUD)"

.PHONY: test
test: test/build
	./$(GTEST_BIN) --gtest_break_on_failure

.PHONY: test/build
test/build: $(GTEST_SRCS) $(GTEST_UNITS) $(GTEST_INIT)
	[ -e $(BIN) ] || mkdir -v $(BIN)
	$(CC) $(CC_FLAGS) $^ -o $(GTEST_BIN) $(GTEST_LIBS) $(LD_FLAGS)

.PHONY: docs
docs:
	./$(DOXYGEN_BIN)

.PHONY: clean
clean:
	rm -vrf $(BIN)
	rm -vrf $(GTEST)/build
	rm -vrf $(DOXYGEN)/build
