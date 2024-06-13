VENDOR = extra/vendor
SKETCH = examples/*
SRC = src

GTEST_DIR = extra/gtest
GTEST_INIT = $(GTEST_DIR)/main.cpp
GTEST_SRC = $(wildcard $(GTEST_DIR)/test_*.cpp)
GTEST_TARGET = bin/gtest

ACC = bin/arduino-cli
ACC_FLAGS = --library $(SRC) --library $(VENDOR)/Servo/src
CC = clang++
CC_FLAGS = -Wall -lgtest

PORT = /dev/ttyUSB0
CORE = arduino:avr
BOARD = arduino:avr:uno
BAUD = 115200

.PHONY: default
default: upload serial

.PHONY: deps
deps:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	$(ACC) core install $(CORE)
	git submodule update --init --recursive

.PHONY: compile
compile:
	$(ACC) compile --verbose $(ACC_FLAGS) --port $(PORT) --fqbn $(BOARD) $(SKETCH)

.PHONY: upload
upload: compile
	$(ACC) upload --verbose $(ACC_FLAGS) --port $(PORT) --fqbn $(BOARD) $(SKETCH)

.PHONY: serial
serial:
	$(ACC) monitor $(ACC_FLAGS) --port $(PORT) --config "baudrate=$(BAUD)"

.PHONY: test
test: test/build
	./$(GTEST_TARGET)

.PHONY: test/build
test/build:
	$(CC) $(CC_FLAGS) $(GTEST_SRC) $(GTEST_INIT) -o $(GTEST_TARGET)

.PHONY: clean
clean:
	rm -vrf bin
