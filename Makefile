PROJECT   := cutecypher
VERSION   := 1.0.0

ROOT      := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
INCLUDE   := $(ROOT)/include
SRCS      := $(wildcard src/*.c)
TESTS     := $(wildcard tests/*.c)
TARGETS   := $(basename $(notdir $(SRCS)))
BUILD_DIR := $(ROOT)/build
LIB_DIR   := $(BUILD_DIR)/lib
LIB_NAME  := $(LIB_DIR)/lib$(PROJECT)

# DEBUG     := 1

CC        := gcc -std=c99
AR        := ar

CFLAGS    += -pedantic -Wall -Wextra -O3 -march=native
CFLAGS    += -I$(INCLUDE)
LDFLAGS   += -shared 

ifdef DEBUG 
	CFLAGS  += -DDEBUG=$(DEBUG)
endif

$(info SRCS are $(SRCS))
$(info TARGETS are $(TARGETS))

.DEFAULT_GOAL = library

prerequisites:
	mkdir -p $(LIB_DIR)

$(CC): prerequisites

test.o: $(TESTS)/main.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN)/test: test.o $(TARGETS).a
	mkdir -p $(BIN)
	$(CC) $^ -o $@

$(TARGETS).o: $(SRCS) $(CC)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

$(LIB_NAME).a: $(TARGETS).o
	$(AR) cr $@ $<

$(LIB_NAME).so.$(VERSION): $(TARGETS).o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
	ln -sf $@ $(LIB_NAME).so

.PHONY: all library test clean

all: library test prerequisites

library: $(LIB_NAME).a $(LIB_NAME).so.$(VERSION)
test: $(BIN)/test

clean:
	rm -rf $(BUILD_DIR)
	rm -rf *.o
