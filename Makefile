LCCC_DEBUG = 1

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L.
CPPFLAGS = -Iinclude

ifeq ($(LCCC_DEBUG),1)
CXXFLAGS += -g -O0
endif

PREFIX ?= /usr/local

DEPS = cppunit

CXXFLAGS += $(shell pkg-config --cflags $(DEPS)) -fPIC
LIBS = -Wl,--as-needed $(shell pkg-config --libs $(DEPS))

TARGET = libccc.so
TESTS = test_lccc

SRC = $(wildcard src/*.cc)
OBJ = $(SRC:%.cc=%.o)

TEST_SRC = $(wildcard tests/*.cc)
TEST_OBJ = $(TEST_SRC:%.cc=%.o)
TEST_LIB = libccc.a

all: $(TARGET) $(TESTS)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS) -shared $(LIBS)

$(TEST_LIB): $(OBJ)
	ar rcs $@ $^

$(TESTS): $(TEST_LIB) $(TEST_OBJ)
	$(CXX) -o $@ $(TEST_OBJ) $(TEST_LIB) $(LDFLAGS) $(LIBS)

run_tests: $(TESTS)
	./$(TESTS)

run_valgrind: $(TESTS)
	LD_LIBRARY_PATH=. valgrind --leak-check=full ./$(TESTS)

run_gdb: $(TESTS)
	LD_LIBRARY_PATH=. gdb ./$(TESTS)

install: all
	install -d $(PREFIX)/lib
	install -m 755 $(TARGET) $(PREFIX)/lib/
	install -d $(PREFIX)/include/lccc
	install -m 644 include/lccc/* $(PREFIX)/include/lccc

clean:
	rm -rf $(OBJ) $(TARGET) $(TEST_OBJ) $(TESTS) $(TEST_LIB)

.PHONY: all clean

