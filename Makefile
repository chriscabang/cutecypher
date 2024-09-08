PROJECT   := cutecypher
VERSION   := 1.0.0

ROOT      := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
SRC       := $(wildcard src/*.c)
OBJ       := $(SRC:.c=.o)
LIBRARY   := lib$(PROJECT)

INCLUDE   := $(ROOT)/include
BUILD     := $(ROOT)/build

LIBPATH   := $(BUILD)/lib

TEST_SRC  := $(wildcard tests/*.c)
TEST_OBJ  := $(TEST_SRC:.c=.o)
TESTS      := $(basename $(TEST_SRC:.c=))

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

prerequisites:
	mkdir -p $(BUILD)/lib

$(CC): prerequisites
$(BUILD): $(CC)

$(TEST_OBJ): 
	$(CC) -Wall -Wextra -g -I$(INCLUDE) -c -o $@ $(dir $@)/$(basename $(notdir $@)).c

$(TESTS): $(LIBRARY).a $(TEST_OBJ)
	$(CC) $@.o $(LIBPATH)/$< -o $(BUILD)/$(basename $(notdir $@))

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

$(LIBRARY).a: $(OBJ)
	$(AR) cr $(LIBPATH)/$@ $<

$(LIBRARY).so.$(VERSION): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(LIBPATH)/$@ $<
	ln -sf $(LIBPATH)/$@ $(LIBRARY).so

.PHONY: all library test clean

all: library test 

library: $(BUILD) $(LIBRARY).a $(LIBRARY).so.$(VERSION)
test: $(BUILD) $(TESTS)

clean:
	find . -name "*.o" -type f -delete
	find . -name "$(LIBRARY).so" -type l -delete
	rm -rf $(BUILD)
