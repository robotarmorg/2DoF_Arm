#
# Makefile to build the project (in case you don't like using the Arduino IDE)
#

ARDUINO = arduino
BOARD   = arduino:avr:uno
PORT    = $(shell find_port.py -n Arduino)

all:
	$(ARDUINO) --verify --board $(BOARD) Arm.ino

upload:
	@echo "port = $(PORT)"
	$(ARDUINO) --upload --board $(BOARD) --port $(PORT) Arm.ino
	usb-ser-mon.py -n Arduino
