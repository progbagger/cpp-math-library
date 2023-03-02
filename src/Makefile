CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror -Wextra -Wshadow -Wpedantic

STATIC_LIB = libmath_objects.a

HEADERS = $(wildcard *.h)

all: $(STATIC_LIB)

$(STATIC_LIB): $(HEADERS)
	@ar -rcs $@ $^

clean:
	@rm -rf *.out *.gch *.o *.a
