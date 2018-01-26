SCC_ROOT_DIR = ..
include $(SCC_ROOT_DIR)/makefile.common

INCLUDE_DIRS := -I$(SDL_INCLUDE_DIR) -I../include

CXXFLAGS := $(INCLUDE_DIRS) $(EXTRA_INCLUDE_DIRS) -std=c++11 $(DEBUG) \
	$(SCC_HAVE_FLAGS)

OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all : $(OBJ)

%.o : %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -f $(OBJ)

.PHONY: clean all
