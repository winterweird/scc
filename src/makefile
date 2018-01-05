SCC_ROOT_DIR = ..
include $(SCC_ROOT_DIR)/makefile.common

CXXFLAGS = -I$(SDL_INCLUDE_DIR) -I../include -std=c++11 $(DEBUG) \
	$(SCC_HAVE_FLAGS)

OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all : $(OBJ)

%.o : %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -f $(OBJ)

.PHONY: clean all
