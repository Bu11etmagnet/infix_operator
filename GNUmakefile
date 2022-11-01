CFLAGS :=
CPPFLAGS :=
CXXFLAGS := -Wall -Wpedantic -Wextra -Werror -std=c++17 -g
LDFLAGS  := -g


# g++     g++-10  g++-12  g++-8   g++-9
# clang++-10 clang++-12
CC := g++-12
#CC := clang++-10
CXX := ${CC}

# The original
# COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

COMPILE2.cpp = $(CXX2) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

CXX2 := clang++-12

%.o: %.cpp
#  recipe to execute (built-in):
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< || $(COMPILE2.cpp) $(OUTPUT_OPTION) $<

all: infix
	./infix

infix: infix.o
# Without this, make uses the built-in cpp -> exe rule,
# not the cpp -> o -> exe chain.

clean:
	rm -f infix infix.o

.PHONY: all clean
