TARGET = prog
lib_location = ../adc0832
LIBS = -lwiringPi -L $(lib_location) -I $(lib_location) -ladc0832
CC = gcc
CFLAGS = -g -Wall

default: $(TARGET)
all: default

prog: leds_and_buttons.o
	$(CC) $(CFLAGS) $< $(LIBS) -o leds_and_buttons

leds_and_buttons.o: leds_and_buttons.c
	$(CC) $(CFLAGS) -c $< $(LIBS) -o leds_and_buttons.o

clean:
	-rm -f leds_and_buttons.o
	-rm -f leds_and_buttons
