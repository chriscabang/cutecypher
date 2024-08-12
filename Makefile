PROJECT   := cutecypher
VERSION   := 1.0.0
TARGET    := lib$(PROJECT)

ROOT      := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
SRC       := $(wildcard src/*.c)
INCLUDE   := $(ROOT)/include
LIB       := $(ROOT)/lib
BIN       := $(ROOT)/bin
TESTS     := $(ROOT)/tests

# DEBUG			:= 1

CC        := gcc -std=c99
AR        := ar

CFLAGS    += -Wall -Wextra -O3 -march=native
CFLAGS    += -I$(INCLUDE)
LDFLAGS   += -shared -Wl -o 

ifdef DEBUG 
	CFLAGS  += -DDEBUG=$(DEBUG)
endif

.DEFAULT_GOAL = library

prerequisites:
	mkdir -p $(LIB)

$(CC): prerequisites

test.o: $(TESTS)/main.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN)/test: test.o $(LIB)/$(TARGET).a
	mkdir -p $(BIN)
	$(CC) $^ -o $@

$(LIB)/$(TARGET).o: $(SRC) $(CC)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

$(LIB)/$(TARGET).a: $(LIB)/$(TARGET).o
	$(AR) cr $@ $<

$(LIB)/$(TARGET).so: $(LIB)/$(TARGET).o
	$(CC) $(CFLAGS) $(LDFLAGS) $@.$(VERSION) $<
	ln -sf `basename $@.$(VERSION)` $@

.PHONY: all library test clean

all: library test prerequisites

library: $(LIB)/$(TARGET).a $(LIB)/$(TARGET).so
test: $(BIN)/test

clean:
	rm -rf $(LIB) $(BIN)
	rm -rf *.out
