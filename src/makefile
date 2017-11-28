# we expect the override flags to be defined and exported to us by the makefile
# above. If this is the toplevel make, we use the default: not overriding.
ifeq (${MAKELEVEL}, 0)
SCC_OVERRIDE_FLAGS =
endif

CXX = g++
CXXFLAGS = -I/usr/include/SDL2 -I../include -std=c++11 $(DEBUG) \
	$(SCC_OVERRIDE_FLAGS)

OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all : $(OBJ)

%.o : %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -f $(OBJ)

.PHONY: clean all
