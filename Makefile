VENDOR = extra/vendor
SKETCHES = examples/*
BIN = bin
SRC = src

ACC = arduino-cli
ACC_FLAGS = --library $(SRC) --library $(VENDOR)/Servo/src

PORT = /dev/ttyUSB0
CORE = arduino:avr
BOARD = arduino:avr:uno
BAUD = 115200

CC = clang++
CC_FLAGS = -Wall -I$(VENDOR)/googletest/googletest/include -I$(VENDOR)/googletest/googletest
LD_FLAGS = -lpthread

GTEST = $(VENDOR)/googletest
GTEST_LIBS = $(GTEST)/build/lib/libgtest.a $(GTEST)/build/lib/libgtest_main.a

GTEST_BIN = $(BIN)/gtest
GTEST_DIR = extra/gtest
GTEST_INIT = $(GTEST_DIR)/main.cpp
GTEST_UNITS = $(wildcard $(GTEST_DIR)/test_*.cpp)
GTEST_SRCS = $(wildcard $(SRC)/*.cpp)

.PHONY: default
default: compile test

.PHONY: deps
deps:
	git submodule update --init --recursive
	mkdir -p $(GTEST)/build && cd $(GTEST)/build && cmake .. && make

.PHONY: compile
compile: $(SKETCHES)
	$(ACC) compile --verbose $(ACC_FLAGS) --port $(PORT) --fqbn $(BOARD) $^

.PHONY: upload
upload: $(SKETCHES)
	$(ACC) upload --verbose $(ACC_FLAGS) --port $(PORT) --fqbn $(BOARD) $^

.PHONY: serial
serial:
	$(ACC) monitor $(ACC_FLAGS) --port $(PORT) --config "baudrate=$(BAUD)"

.PHONY: test
test: test/build
	./$(GTEST_BIN)

.PHONY: test/build
test/build: $(GTEST_SRCS) $(GTEST_UNITS) $(GTEST_INIT)
	[ -e $(BIN) ] || mkdir -v $(BIN)
	$(CC) $(CC_FLAGS) $^ -o $(GTEST_BIN) $(GTEST_LIBS) $(LD_FLAGS)

.PHONY: clean
clean:
	rm -vrf bin
	rm -vrf $(GTEST)/build
