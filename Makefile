PROJECT   := cutecypher
VERSION   := 1.0.0

ROOT      := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
INCLUDE   := $(ROOT)/include
SRCS      := $(wildcard src/*.c)
TARGET_SRC:= $(basename $(notdir $(SRCS)))
TSTS      := $(wildcard tests/*.c)
TARGET_TST:= $(basename $(notdir $(TSTS)))

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

.DEFAULT_GOAL = library

# $(info SRCS is $(SRCS))
# $(info TSTS is $(TSTS))
# $(info TARGET_SRC is $(TARGET_SRC))
# $(info TARGET_TST is $(TARGET_TST))

prerequisites:
	mkdir -p $(LIB_DIR)

$(CC): prerequisites

$(TARGET_TST).o: $(TSTS)
	$(CC)  -Wall -Wextra -g -I$(INCLUDE) -I./tests -c -o $@ $<

$(BUILD_DIR)/$(TARGET_TST): $(TARGET_TST).o $(LIB_NAME).a
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@

$(TARGET_SRC).o: $(SRCS) $(CC)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

$(LIB_NAME).a: $(TARGET_SRC).o
	$(AR) cr $@ $<

$(LIB_NAME).so.$(VERSION): $(TARGET_SRC).o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
	ln -sf $@ $(LIB_NAME).so

.PHONY: all library test clean

all: library test prerequisites

library: $(LIB_NAME).a $(LIB_NAME).so.$(VERSION)
test: $(BUILD_DIR)/$(TARGET_TST)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf *.o
