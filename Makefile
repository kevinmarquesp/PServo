ARDUINO_CLI = bin/arduino-cli
SKETCH = examples/*
SRC = src

VENDOR = extra/vendor
LIBS = --library $(SRC) --library $(VENDOR)/Servo/src

PORT = /dev/ttyUSB0
CORE = arduino:avr
BOARD = arduino:avr:uno
BAUD = 115200

.PHONY: deps
deps:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	$(ARDUINO_CLI) core install $(CORE)
	git submodule update --init --recursive

.PHONY: default
default: upload serial

.PHONY: compile
compile:
	$(ARDUINO_CLI) compile --verbose $(LIBS) --port $(PORT) --fqbn $(BOARD) $(SKETCH)

.PHONY: upload
upload: compile
	$(ARDUINO_CLI) upload --verbose $(LIBS) --port $(PORT) --fqbn $(BOARD) $(SKETCH)

.PHONY: serial
serial:
	$(ARDUINO_CLI) monitor $(LIBS) --port $(PORT) --config "baudrate=$(BAUD)"