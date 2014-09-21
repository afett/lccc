LCCC_DEBUG = 1

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L.
CPPFLAGS = -Iinclude

ifeq ($(LCCC_DEBUG),1)
CXXFLAGS += -g -O0
endif

DEPS = cppunit

CXXFLAGS += $(shell pkg-config --cflags $(DEPS)) -fPIC
LIBS = -Wl,--as-needed $(shell pkg-config --libs $(DEPS))

TARGET = libccc.so
TESTS = test_lccc

SRC = $(wildcard src/*.cc)
OBJ = $(SRC:%.cc=%.o)

TEST_SRC = $(wildcard tests/*.cc)
TEST_OBJ = $(TEST_SRC:%.cc=%.o)

all: $(TARGET) $(TESTS)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS) -shared $(LIBS)

$(TESTS): $(TARGET) $(TEST_OBJ)
	$(CXX) -o $@ $(TEST_OBJ) $(LDFLAGS) $(LIBS) -lccc

run_tests: $(TESTS)
	LD_LIBRARY_PATH=. ./$(TESTS)

run_valgrind: $(TESTS)
	LD_LIBRARY_PATH=. valgrind --leak-check=full ./$(TESTS)

run_gdb: $(TESTS)
	LD_LIBRARY_PATH=. gdb ./$(TESTS)

clean:
	rm -rf $(OBJ) $(TARGET) $(TEST_OBJ) $(TESTS)

.PHONY: all clean

